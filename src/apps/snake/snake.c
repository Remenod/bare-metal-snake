#include "snake.h"

#include <stdbool.h>
#include <keyboard.h>
#include <screen.h>
#include <string.h>
#include <stdint.h>
#include <arrlib.h>
#include <pit.h>

char buf[12];

uint16_t snake_size, tail_end_shift, head_pos, apple_pos;
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
        put_string(1920, "                                                                               ");
        // put_string(1930, int_to_str(tail[0], buf));
        // put_string(1940, int_to_str(tail[1], buf));
        // put_string(1950, int_to_str(tail[2], buf));
        // put_string(1960, int_to_str(tail[3], buf));
        put_string(1970, int_to_str(head_pos, buf));
        put_string(1980, int_to_str(snake_size, buf));

        if (contains(tail, snake_size, head_pos))
            return; // TODO improve lose routine

        put_char(head_pos - 1, 219);
        put_char(head_pos, 219); // 177 178 good also
        put_char(tail[tail_end_shift] - 1, 0);
        put_char(tail[tail_end_shift], 0);

        tail[tail_end_shift] = head_pos;

        tail_end_shift++;
        tail_end_shift %= snake_size;
    }
}