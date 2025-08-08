#include <screen.h>

#include <ports.h>
#include <string.h>

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

void disable_vga_cursor()
{
    outb(0x3D4, 0x0A);
    outb(0x3D5, 0x20);
}

void move_cursor(uint16_t pos)
{
    outb(0x3D4, 0x0F);
    outb(0x3D5, pos & 0xFF);
    outb(0x3D4, 0x0E);
    outb(0x3D5, (pos >> 8) & 0xFF);
}