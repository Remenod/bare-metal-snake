#include <screen.h>

#include <ports.h>
#include <string.h>
#include <stdbool.h>

volatile uint16_t *vga = (volatile uint16_t *)0xB8000;

void put_char(int pos, char c)
{
    vga[pos] = (0x0F << 8) | c;
}

void clear_screen()
{
    uint16_t blank = (0x07 << 8) | ' ';
    for (int i = 0; i < 80 * 25; i++)
    {
        vga[i] = blank;
    }
}

void put_string(int start_pos, char text[])
{
    int endpoint = start_pos + strlen(text);
    for (int i = start_pos; i < endpoint; i++)
    {
        put_char(i, text[i - start_pos]);
    }
}

void set_cursor_visibility(bool_t visible)
{
    outb(0x3D4, 0x0A);
    uint8_t cursor_start = inb(0x3D5);

    if (visible)
        cursor_start &= ~0x20;
    else
        cursor_start |= 0x20;

    outb(0x3D4, 0x0A);
    outb(0x3D5, cursor_start);
}

void move_cursor(uint16_t pos)
{
    outb(0x3D4, 0x0F);
    outb(0x3D5, pos & 0xFF);
    outb(0x3D4, 0x0E);
    outb(0x3D5, (pos >> 8) & 0xFF);
}