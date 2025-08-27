#include <drivers/mouse.h>

#include <interrupts/isr.h>
#include <ports.h>
#include <drivers/screen.h>
#include <lib/string.h>
#include <interrupts/pic.h>

#define PS2_DATA_PORT 0x60
#define PS2_STATUS_PORT 0x64
#define PS2_CMD_PORT 0x64

#define MOUSE_CHAR 167

static mouse_packet_t last_packet;
static uint8_t buf[13];
static uint8_t mouse_idx = 0;

static uint16_t mouse_x = 0, mouse_y = 0, mouse_cover_buf;

void mouse_handler(void)
{
    uint8_t data = inb(PS2_DATA_PORT);

    buf[mouse_idx++] = data;

    if (mouse_idx == 3)
    {
        mouse_idx = 0;

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

    if (!(get_char(mouse_y * 80 + mouse_x) - MOUSE_CHAR))
        put_attrchar(mouse_x + mouse_y * 80, mouse_cover_buf);
    int new_x = mouse_x + packet->dx / 3;
    int new_y = mouse_y - packet->dy / 3;

    new_x = new_x < 0 ? 0 : new_x;
    mouse_x = new_x > 79 ? 79 : new_x;

    new_y = new_y < 0 ? 0 : new_y;
    mouse_y = new_y > 24 ? 24 : new_y;

    mouse_cover_buf = get_attrchar(mouse_x + mouse_y * 80);
    put_char(mouse_x + mouse_y * 80, MOUSE_CHAR);
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
