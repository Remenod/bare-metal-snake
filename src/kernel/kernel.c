#include <stdint.h>
#include <stddef.h>
#include <string.h>

volatile uint16_t *vga = (volatile uint16_t *)0xB8000;

void put_char(int pos, char c)
{
    vga[pos] = (0x0F << 8) | c;
}

void put_string(int start_pos, char text[])
{
    int endpoint = start_pos + strlen(text);
    for (int i = start_pos; i < endpoint; i++)
    {
        put_char(i, text[i - start_pos]);
    }
}

void clear_screen()
{
    uint16_t blank = (0x07 << 8) | ' ';
    for (int i = 0; i < 80 * 25; i++)
    {
        vga[i] = blank;
    }
}

void kernel_main()
{
    clear_screen();
    char hello[] = "Hello, world!";

    put_string(80 * 25 / 2 - strlen(hello) / 2, hello);
    while (1)
        ;
}