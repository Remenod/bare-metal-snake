#include "expr_eval.h"

#include <lib/string.h>
#include <drivers/keyboard.h>
#include <drivers/screen.h>
#include <kernel/diagnostics/rsod_routine.h>

#define SCREEN_WIDTH 80
#define STACK_SIZE 32

static void print_error(char const* msg)
{
    for (int j = 80; j < 160; j++)
        put_char(j, 0);
    put_string(80, "error: ");
    put_string(87, msg);
}

void eval_expr_main(void)
{
    set_vga_cursor_visibility(true);
    set_vga_cursor_pos(0);
    char buf[SCREEN_WIDTH];
    int buf_idx = 0;
    int stack[STACK_SIZE];

    while (true)
    {
        char c;
        while (!(c = get_keyboard_char()))
            asm volatile("hlt");

        switch (c)
        {
        case KEY_ESC:
            return;

        case '\b':
            if (buf_idx > 0 && get_vga_cursor_pos() > 0)
                buf_idx--;
            set_vga_cursor_pos(get_vga_cursor_pos() - 1);
            print_char('\0');
            set_vga_cursor_pos(get_vga_cursor_pos() - 1);
            break;

        case '\n':
            buf[buf_idx] = '\0';
            size_t n = eval_expr(buf, stack, STACK_SIZE, print_error);
            for (size_t i = 0; i < n; ++i)
            {
                char buf[12];
                for (int j = 80; j < 160; j++)
                    put_char(j, 0);
                put_string(80, int_to_str(stack[i], buf));
            }
            break;

        default:
            print_char(c);
            buf[buf_idx++] = c;
            break;
        }
    }
}
