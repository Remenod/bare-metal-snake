#include <drivers/mouse.h>

#include <interrupts/isr.h>
#include <ports.h>
#include <drivers/screen.h>
#include <lib/string.h>
#include <interrupts/pic.h>
#include <drivers/vga.h>
#include <drivers/qemu_serial.h>
#include <lib/math.h>

#define PS2_DATA_PORT 0x60
#define PS2_STATUS_PORT 0x64
#define PS2_CMD_PORT 0x64

#define MOUSE_CHAR_1 0xA6
#define MOUSE_CHAR_2 0xA7
#define MOUSE_CHAR_3 0xEC
#define MOUSE_CHAR_4 0xED

static mouse_packet_t last_packet;
static uint8_t buf[13], cursor_cover_buf[4];
static uint8_t mouse_packet_index = 0;

static const uint8_t mouse_glyphs_codes[] = {MOUSE_CHAR_1, MOUSE_CHAR_2, MOUSE_CHAR_3, MOUSE_CHAR_4};

static uint8_t cursor_glyph[] =
    {0b10000000,
     0b11000000,
     0b11100000,
     0b11110000,
     0b11111000,
     0b11111100,
     0b11111110,
     0b11111110,
     0b11110000,
     0b11011000,
     0b10011000,
     0b00001100,
     0b00001100,
     0x00000000,
     0x00000000,
     0b00000000};

static uint16_t mouse_x = 0, mouse_y = 0;

void mouse_handler(void)
{
    uint8_t data = inb(PS2_DATA_PORT);

    buf[mouse_packet_index++] = data;

    if (mouse_packet_index == 3)
    {
        mouse_packet_index = 0;

        last_packet.buttons = buf[0] & 0b111; // &0b001 - left, &0b010 - right, &0b100 - middle,
        last_packet.dx = (int8_t)buf[1];
        last_packet.dy = (int8_t)buf[2];

        mouse_process(&last_packet);
        return;
    }

    outb(0xA0, 0x20); // EOI slave
    outb(0x20, 0x20); // EOI master
}

void mouse_process(mouse_packet_t *packet)
{
    put_char(0, (packet->buttons & 0b001));
    put_char(1, (packet->buttons & 0b010) >> 1);
    put_char(2, (packet->buttons & 0b100) >> 2);

    uint16_t operating_chars_pos[4];
    operating_chars_pos[0] = mouse_x / 8 + mouse_y / 16 * 80;
    operating_chars_pos[1] = operating_chars_pos[0] + 1;
    operating_chars_pos[2] = operating_chars_pos[0] + 80;
    operating_chars_pos[3] = operating_chars_pos[0] + 81;

    for (int i = 0; i < 4; i++)
        if (!(get_char(operating_chars_pos[i]) - mouse_glyphs_codes[i]))
            put_char(operating_chars_pos[i], cursor_cover_buf[i]);

    int new_x = mouse_x + packet->dx;
    int new_y = mouse_y - packet->dy;
    new_x = new_x < 0 ? 0 : new_x;
    mouse_x = new_x > (79 * 8) ? (79 * 8) : new_x;
    new_y = new_y < 0 ? 0 : new_y;
    mouse_y = new_y > (24 * 16) ? (24 * 16) : new_y;

    operating_chars_pos[0] = mouse_x / 8 + mouse_y / 16 * 80;
    operating_chars_pos[1] = operating_chars_pos[0] + 1;
    operating_chars_pos[2] = operating_chars_pos[0] + 80;
    operating_chars_pos[3] = operating_chars_pos[0] + 81;

    uint8_t mouse_glyph_buf[4][16] = {0};

    uint8_t cursor_char_offset_x = mouse_x % 8;
    uint8_t cursor_char_offset_y = mouse_y % 16;

    for (int i = 0; i < 4; i++)
    {
        uint8_t *covering_glyph = get_8x16_font_glyph(get_char(operating_chars_pos[i]));

        int8_t cur_glyph_offset_y;

        for (int j = 0; j < 16; j++)
        {
            cur_glyph_offset_y = j - cursor_char_offset_y + (i < 2 ? 0 : 16);
            mouse_glyph_buf[i][j] =
                covering_glyph[j] | ((cur_glyph_offset_y < 0 || cur_glyph_offset_y > 15)
                                         ? 0
                                         : (i % 2
                                                ? (cursor_glyph[cur_glyph_offset_y] << (8 - cursor_char_offset_x))
                                                : (cursor_glyph[cur_glyph_offset_y] >> cursor_char_offset_x)));
        }
    }
    write_glyphs(4, mouse_glyph_buf, mouse_glyphs_codes);

    for (int i = 0; i < 4; i++)
    {
        cursor_cover_buf[i] = get_char(operating_chars_pos[i]);
        put_char(operating_chars_pos[i], mouse_glyphs_codes[i]);
    }
}

static inline void ps2_wait_input_empty()
{
    while (inb(PS2_STATUS_PORT) & 0x02)
        ;
}

static void ps2_mouse_send(uint8_t cmd)
{
    ps2_wait_input_empty();
    outb(PS2_CMD_PORT, 0xD4);
    ps2_wait_input_empty();
    outb(PS2_DATA_PORT, cmd);
}

void mouse_install(void)
{
    pic_unmask_irq(2);
    pic_unmask_irq(12);

    ps2_wait_input_empty();
    outb(PS2_CMD_PORT, 0xA8);

    ps2_wait_input_empty();
    outb(PS2_CMD_PORT, 0x20);
    uint8_t status = inb(PS2_DATA_PORT);
    status |= 0x02;
    ps2_wait_input_empty();
    outb(PS2_CMD_PORT, 0x60);
    ps2_wait_input_empty();
    outb(PS2_DATA_PORT, status);

    ps2_mouse_send(0xFF);
    (void)inb(PS2_DATA_PORT);
    (void)inb(PS2_DATA_PORT);
    (void)inb(PS2_DATA_PORT);

    ps2_mouse_send(0xF4);
    (void)inb(PS2_DATA_PORT);
    (void)inb(PS2_DATA_PORT);
    (void)inb(PS2_DATA_PORT);
    (void)inb(PS2_DATA_PORT);

    register_interrupt_handler(44, mouse_handler);
}
