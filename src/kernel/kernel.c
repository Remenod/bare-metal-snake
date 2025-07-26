#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <keyboard.h>
#include <screen.h>

void kernel_main()
{
    clear_screen();

    idt_install();
    keyboard_install();

    char hello[] = "Hello, world!";

    put_string(80 * 25 / 2 - strlen(hello) / 2, hello);
    int i = 3;
    move_cursor(i);
    while (true)
    {
        char c;
        while (!(c = get_char()))
        {
            asm volatile("hlt");
        }

        switch (c)
        {
        case '\b':
            i--;
            put_char(i, 0);
            break;
        case '\t':
            put_string(i, "    ");
            i += 4;
            break;
        case '\n':
            if (i % 80 == 0)
                i += 80;
            else
                i += 80 - (i % 80);
            break;
        case KEY_UP:
            if (i >= 80)
                i -= 80;
            break;
        case KEY_DOWN:
            if (i <= 80 * 24)
                i += 80;
            break;
        case KEY_LEFT:
            if (i > 0)
                i--;
            break;
        case KEY_RIGHT:
            if (i <= 80 * 25)
                i++;
            break;
        default:
            put_char(i, c);
            i++;
            break;
        }
        move_cursor(i);
        if (i > 80 * 25)
            i = 3;
    }
}