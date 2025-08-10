#include "snake.h"

#include <stdbool.h>
#include <keyboard.h>
#include <screen.h>
#include <string.h>
#include <stdint.h>
#include <arrlib.h>

char buf[12];

uint16_t snake_size, tail_end_shift, head_pos;
uint16_t tail_end_shift;
uint16_t tail[2000];

void snake_main()
{
    snake_size = 4;
    tail_end_shift = 0;
    tail[0] = 0;
    tail[1] = 2;
    tail[2] = 4;
    head_pos = 6;

    while (true)
    {
        char c;
        while (!(c = get_char()))
            asm volatile("hlt");

        switch (c)
        {
        case 27:
            return;
        case '\n':
            snake_size++;
            put_string(1980, int_to_str(snake_size, buf));
            continue;
        case KEY_UP:
            if (head_pos >= 80)
                head_pos -= 80;
            break;
        case KEY_DOWN:
            if (head_pos <= 80 * 24)
                head_pos += 80;
            break;
        case KEY_LEFT:
            if (head_pos > 0)
                head_pos -= 2;
            break;
        case KEY_RIGHT:
            if (head_pos <= 80 * 25)
                head_pos += 2;
            break;
        }
        if (contains(tail, snake_size, head_pos))
            return; // TODO improve lose detection and routine

        put_char(head_pos - 1, 160);
        put_char(head_pos, 160);

        put_char(tail[tail_end_shift] - 1, 0);
        put_char(tail[tail_end_shift], 0);

        tail[tail_end_shift] = head_pos;

        tail_end_shift++;
        tail_end_shift %= snake_size;

        put_string(1980, int_to_str(snake_size, buf));
    }
}