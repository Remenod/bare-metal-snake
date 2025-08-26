#include "text_sandbox.h"

#include <lib/types.h>
#include <drivers/keyboard.h>
#include <drivers/screen.h>
#include <lib/string.h>

void text_sandbox_main()
{
    set_cursor_visibility(true);
    clear_screen();

    for (int n = 0; n < 256; n++)
        put_char(n, n);

    static const char hello[] = "Hello, world!";
    put_string(80 * 25 / 2 - strlen(hello) / 2, hello);
    int i = 0;
    set_cursor_pos(i);
    while (true)
    {
        char c;
        while (!(c = get_char()))
            asm volatile("hlt");

        switch (c)
        {
        case KEY_ESC:
            return;
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
        set_cursor_pos(i);
        if (i > 80 * 25)
            i = 3;
    }
}