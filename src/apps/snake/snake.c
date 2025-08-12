#include "snake.h"

#include <stdbool.h>
#include <keyboard.h>
#include <screen.h>
#include <string.h>
#include <stdint.h>
#include <arrlib.h>
#include <pit.h>

char buf[12];

char last_key;

char score_text[12];

const char *lose_text[] = {
    "You lose!",
    score_text,
    "Press R to restart",
    "Press ESC to go app selector"};

const int lose_text_size = 4;

uint64_t ticks_on_last_automove;
uint16_t snake_size, tail_end_shift, head_pos, apple_pos, game_speed;
uint16_t tail[1000];

uint16_t get_random_odd_apple_pos()
{
    uint16_t res = (uint32_t)get_timer_ticks() % 1920;
    res = res - (res % 2) + 1;
    if (res == head_pos || contains(tail, snake_size, res))
        return get_random_odd_apple_pos();
    else
        return res;
}

void show_info(int data[6])
{
    put_string(1920, "                                                                               ");
    put_string(1930, int_to_str(data[0], buf));
    put_string(1940, int_to_str(data[1], buf));
    put_string(1950, int_to_str(data[2], buf));
    put_string(1960, int_to_str(data[3], buf));
    put_string(1970, int_to_str(data[4], buf));
    put_string(1980, int_to_str(data[5], buf));
}

void draw_snake()
{
    put_char(head_pos - 1, 219);
    put_char(head_pos, 219);
}

void erase(uint16_t pos)
{
    put_char(pos - 1, 0);
    put_char(pos, 0);
}

void draw_apple()
{
    put_char(apple_pos - 1, 177);
    put_char(apple_pos, 177);
}

void print_lose()
{
    strcpy(score_text, "Score: ");
    strcat(score_text, int_to_str(snake_size, buf));
    for (int i = 0; i < lose_text_size; i++)
        put_string((80 * 25 / 2 - strlen(lose_text[i]) / 2) + 80 * (i - 2), lose_text[i]);
}

void snake_main()
{
    set_cursor_visibility(false);
restart:
    clear_screen();
    ticks_on_last_automove = get_timer_ticks();
    last_key = KEY_RIGHT;
    snake_size = 4;
    tail_end_shift = 0;
    tail[0] = 1;
    tail[1] = 3;
    tail[2] = 5;
    head_pos = 7;
    apple_pos = get_random_odd_apple_pos();
    game_speed = 300;

    draw_apple();

    while (true)
    {
        char c;
        while (!(c = get_char()))
        {
            if (ticks_on_last_automove + game_speed < get_timer_ticks())
            {
                c = last_key;
                ticks_on_last_automove = get_timer_ticks();
                break;
            }
            show_info((int[]){ticks_on_last_automove, get_timer_ticks(), 0, apple_pos, head_pos, snake_size});
            asm volatile("hlt");
        }
        switch (c)
        {
        case 27:
            return;
        case KEY_UP:
            if (last_key == KEY_DOWN)
                continue;
            if (head_pos >= 80)
                head_pos -= 80;
            last_key = KEY_UP;
            break;
        case KEY_DOWN:
            if (last_key == KEY_UP)
                continue;
            if (head_pos <= 80 * 24)
                head_pos += 80;
            last_key = KEY_DOWN;
            break;
        case KEY_LEFT:
            if (last_key == KEY_RIGHT)
                continue;
            if (head_pos > 0)
                head_pos -= 2;
            last_key = KEY_LEFT;
            break;
        case KEY_RIGHT:
            if (last_key == KEY_LEFT)
                continue;
            if (head_pos <= 80 * 24)
                head_pos += 2;
            last_key = KEY_RIGHT;
            break;
        default:
            continue;
        }
        ticks_on_last_automove = get_timer_ticks();
        if (head_pos > 1920)
            head_pos = 1921; // TODO make this thing smarter

        if (contains(tail, snake_size, head_pos))
        {
            print_lose();
            while (true)
            {
                while (!(c = get_char()))
                    asm volatile("hlt");
                switch (c)
                {
                case 27:
                    return;
                case 'r':
                    goto restart;
                }
            }
        }

        if (head_pos == apple_pos)
        {
            snake_size++;
            apple_pos = get_random_odd_apple_pos();
            draw_apple();
        }

        draw_snake();
        erase(tail[tail_end_shift]);

        tail[tail_end_shift] = head_pos;
        tail_end_shift++;
        tail_end_shift %= snake_size;

        show_info((int[]){ticks_on_last_automove, get_timer_ticks(), 0, apple_pos, head_pos, snake_size});
    }
}