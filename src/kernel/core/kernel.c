#include <interrupts/idt.h>
#include <drivers/screen.h>
#include <drivers/keyboard.h>
#include <timer/pit.h>
#include <drivers/vga.h>
#include <lib/mem.h>
#include <lib/string.h>
#include "../../apps/app_selector/app_selector.h"

#define VGA_MEM 0xA0000
#define WIDTH 320
#define HEIGHT 200

#define COM1_PORT 0x3F8

#define VGA_SIZE (WIDTH * HEIGHT / 6)

uint8_t saved_screen[VGA_SIZE] __attribute__((section(".bss.vga")));
// static uint8_t saved_screen1[VGA_SIZE / 2];
// static uint8_t saved_screen2[VGA_SIZE];

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

    // uint8_t font_buf[256][FONT_HEIGHT];

    // read_font(font_buf);
    // send_font(font_buf);

    // sleep(5000);
    char buf[12];
    put_string(1920, int_to_str(&saved_screen, buf));
    sleep(1000);
    demo_graphics();

    // sleep(2000);
    save_screen(0);
    // sleep(2000);
    draw_mode13h_test_pattern();
    sleep(2000);
    restore_screen(0);

    // read_font(font_buf);
    // send_font(font_buf);
    // read_regs(dump);

    // serial_write_dump_hex(dump, 61);

    // sleep(5000);
    // write_regs(dump);

    set_text_m();

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
// #define OFFSET (10666 * 0)

#define OFFSET (00)

void save_screen(int part)
{
    uint8_t *vga = (uint8_t *)(VGA_MEM + OFFSET);
    for (int i = 0; i < VGA_SIZE * 2; i += 2)
    {
        saved_screen[i / 2] = vga[i];
    }
}

void restore_screen(int part)
{
    uint8_t *vga = (uint8_t *)(VGA_MEM + OFFSET);
    for (int i = 0; i < VGA_SIZE * 2; i += 2)
    {
        vga[i] = saved_screen[i / 2];
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