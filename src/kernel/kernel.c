#include <interrupts/idt.h>
#include <drivers/screen.h>
#include <drivers/keyboard.h>
#include <timer/pit.h>
#include <string.h>
#include "../apps/app_selector/app_selector.h"

#include "../drivers/vga.h"

#define VGA_MEM 0xA0000
#define WIDTH 320
#define HEIGHT 200

void kernel_main()
{
    idt_install();
    keyboard_install();

    pit_init(1000);

    // unsigned char dump[61];

    // draw_mode13h_test_pattern();
    // read_regs(dump);

    // serial_write_dump_hex(dump, 61);
    // sleep(2000);
    // print("OKKKKKKKKKKKKKKKKKKK");
    // sleep(2000);
    // serial_write_char('\n');
    // serial_write_char('\n');
    uint8_t font_buf[256][FONT_HEIGHT];

    read_font(font_buf);
    send_font(font_buf);

    // sleep(5000);
    // demo_graphics();

    // draw_mode13h_test_pattern();

    read_font(font_buf);
    send_font(font_buf);
    // read_regs(dump);

    // serial_write_dump_hex(dump, 61);

    // sleep(5000);
    // write_regs(dump);

    // set_text_m();

    // serial_write_char('\n');
    // serial_write_char('\n');

    // unsigned char buf[4096];
    // read_bios_font(buf);
    // serial_write_dump_hex(buf, 4096);

    app_selector();
    // uint8_t palette[256][3];
    // read_palette(palette);
    // send_palette(palette);
}

#define COM1_PORT 0x3F8

void serial_init()
{
    outb(COM1_PORT + 1, 0x00); // Disable all interrupts
    outb(COM1_PORT + 3, 0x80); // Enable DLAB (set baud rate divisor)
    outb(COM1_PORT + 0, 0x03); // Set divisor to 3 (lo byte) 38400 baud if 115200 base
    outb(COM1_PORT + 1, 0x00); //                  (hi byte)
    outb(COM1_PORT + 3, 0x03); // 8 bits, no parity, one stop bit
    outb(COM1_PORT + 2, 0xC7); // Enable FIFO, clear them, with 14-byte threshold
    outb(COM1_PORT + 4, 0x0B); // IRQs enabled, RTS/DSR set
}

int serial_is_transmit_empty()
{
    return inb(COM1_PORT + 5) & 0x20;
}

void serial_write_char(char c)
{
    while (!serial_is_transmit_empty())
        ;
    outb(COM1_PORT, c);
}

void serial_write_hex_byte(unsigned char byte)
{
    const char hex_digits[] = "0123456789ABCDEF";
    serial_write_char(hex_digits[(byte >> 4) & 0xF]);
    serial_write_char(hex_digits[byte & 0xF]);
}

void serial_write_dump_hex(const unsigned char *dump, int length)
{
    for (int i = 0; i < length; i++)
    {
        serial_write_char('0');
        serial_write_char('x');
        serial_write_hex_byte(dump[i]);
        serial_write_char(',');
        serial_write_char(' ');
    }
}

void serial_write_str(const char *s)
{
    while (*s)
        serial_write_char(*s++);
}

void serial_write_uint8(uint8_t value)
{
    char buf[4];
    int i = 0;

    if (value == 0)
    {
        serial_write_char('0');
        return;
    }

    while (value > 0)
    {
        buf[i++] = '0' + (value % 10);
        value /= 10;
    }

    while (i--)
        serial_write_char(buf[i]);
}

void send_palette(uint8_t palette[256][3])
{
    for (int i = 0; i < 256; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            serial_write_uint8(palette[i][j]);
            if (j < 2)
                serial_write_char(' ');
        }
        serial_write_str("\r\n");
    }
}

void send_font(uint8_t font[256][FONT_HEIGHT])
{
    char print_dec_buf[12];
    for (int i = 0; i < 256; i++)
    {
        serial_write_str(int_to_str(i, print_dec_buf));
        serial_write_str(":  ");
        for (int j = 0; j < FONT_HEIGHT; j++)
        {

            serial_write_hex_byte(font[i][j]);
            if (j < FONT_HEIGHT - 1)
                serial_write_char(' ');
        }
        serial_write_str("\r\n");
    }
}

void draw_mode13h_test_pattern()
{
    volatile uint8_t *vga = (volatile uint8_t *)VGA_MEM;
    for (int y = 0; y < HEIGHT; y++)
    {
        for (int x = 0; x < WIDTH; x++)
        {
            vga[y * WIDTH + x] = (uint8_t)((x + y) & 0xFF);
        }
    }
}
