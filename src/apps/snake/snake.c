#include "snake.h"

#include <stdbool.h>
#include <keyboard.h>
#include <screen.h>
#include <string.h>
#include <stdint.h>
#include <arrlib.h>
#include <pit.h>
#include <random.h>

#define FIELD_HEIGHT 25
#define FIELD_WIDTH 80

#define SNAKE_SYMBOL 219
#define APPLE_SYMBOL 177

static char last_key, buf[12], score_text[12];

static const char game_end_win[] = "You win!",
                  game_end_lose[] = "You lose!",
                  *lose_text[] = {
                      score_text,
                      "Press R to restart",
                      "Press ESC to go app selector"};

enum
{
    LOSE_TEXT_SIZE = 3
};

static uint16_t snake_size, tail_end_shift, head_pos, apple_pos, game_speed, tail[(FIELD_HEIGHT * FIELD_WIDTH) / 2];
static uint32_t timer_frequency;
static uint64_t ticks_on_last_automove;

static Random rand;

static uint16_t get_new_apple_pos()
{
gnap:
    uint16_t res = random_next_bounded(&rand, FIELD_HEIGHT * FIELD_WIDTH);
    res = res - (res % 2) + 1;
    if (res == head_pos || contains(tail, snake_size, res))
        goto gnap;
    else
        return res;
}

static inline void draw_snake()
{
    put_char(head_pos - 1, SNAKE_SYMBOL);
    put_char(head_pos, SNAKE_SYMBOL);
}

static inline void draw_apple()
{
    put_char(apple_pos - 1, APPLE_SYMBOL);
    put_char(apple_pos, APPLE_SYMBOL);
}

static inline void erase(uint16_t pos)
{
    put_char(pos - 1, 0);
    put_char(pos, 0);
}

static void print_game_end(bool_t is_win)
{
    if (is_win)
        put_string((FIELD_WIDTH * FIELD_HEIGHT / 2 - strlen(game_end_win) / 2) + FIELD_WIDTH * -3, game_end_win);
    else
        put_string((FIELD_WIDTH * FIELD_HEIGHT / 2 - strlen(game_end_lose) / 2) + FIELD_WIDTH * -3, game_end_lose);
    strcpy(score_text, "Score: ");
    strcat(score_text, int_to_str(snake_size, buf));
    for (int i = 0; i < LOSE_TEXT_SIZE; i++)
        put_string((FIELD_WIDTH * FIELD_HEIGHT / 2 - strlen(lose_text[i]) / 2) + FIELD_WIDTH * (i - 2), lose_text[i]);
}

static inline bool_t is_opposite_direction(char new_dir, char old_dir)
{
    return (new_dir == KEY_UP && old_dir == KEY_DOWN) ||
           (new_dir == KEY_DOWN && old_dir == KEY_UP) ||
           (new_dir == KEY_LEFT && old_dir == KEY_RIGHT) ||
           (new_dir == KEY_RIGHT && old_dir == KEY_LEFT);
}

static void game_reset()
{
    clear_screen();
    static const uint16_t start_tail[] = {1, 3, 5};
    ticks_on_last_automove = get_timer_ticks();
    last_key = KEY_RIGHT;
    snake_size = sizeof(start_tail) / sizeof(start_tail[0]) + 1;
    tail_end_shift = 0;
    memcpy(tail, start_tail, sizeof(start_tail));
    head_pos = 7;
    apple_pos = get_new_apple_pos();
    game_speed = 4;
    draw_apple();
}

void snake_main()
{
    set_cursor_visibility(false);
    random_init(&rand, get_timer_ticks());
    timer_frequency = get_timer_frequency();
restart:
    game_reset();

    while (true)
    {
        char c;
        while (!(c = get_char()))
        {
            if (ticks_on_last_automove + (timer_frequency / game_speed) < get_timer_ticks())
            {
                c = last_key;
                ticks_on_last_automove = get_timer_ticks();
                break;
            }
            asm volatile("hlt");
        }
        switch (c)
        {
        case KEY_ESC:
            return;

        case KEY_UP:
        case KEY_DOWN:
        case KEY_LEFT:
        case KEY_RIGHT:
            if (is_opposite_direction(c, last_key))
                continue;

            switch (c)
            {
            case KEY_UP:
                if (head_pos >= FIELD_WIDTH)
                    head_pos -= FIELD_WIDTH;
                else
                    head_pos += (FIELD_HEIGHT - 1) * FIELD_WIDTH;
                break;
            case KEY_DOWN:
                if (head_pos + FIELD_WIDTH < FIELD_WIDTH * FIELD_HEIGHT)
                    head_pos += FIELD_WIDTH;
                else
                    head_pos -= (FIELD_HEIGHT - 1) * FIELD_WIDTH;
                break;
            case KEY_LEFT:
                if (head_pos % FIELD_WIDTH > 0)
                    head_pos -= 2;
                else
                    head_pos += FIELD_WIDTH - 2;
                break;
            case KEY_RIGHT:
                if (head_pos % FIELD_WIDTH < FIELD_WIDTH - 2)
                    head_pos += 2;
                else
                    head_pos -= FIELD_WIDTH - 2;
                break;
            }

            last_key = c;
            break;

        default:
            continue;
        }
        ticks_on_last_automove = get_timer_ticks();

        if (contains(tail, snake_size, head_pos) || snake_size > FIELD_HEIGHT * FIELD_WIDTH / 2 - 1)
        {
            print_game_end(snake_size > FIELD_HEIGHT * FIELD_WIDTH / 2 - 1);
            while (true)
            {
                while (!(c = get_char()))
                    asm volatile("hlt");
                switch (c)
                {
                case KEY_ESC:
                    return;
                case 'r':
                    goto restart;
                }
            }
        }

        if (head_pos == apple_pos)
        {
            snake_size++;
            apple_pos = get_new_apple_pos();
            draw_apple();
        }

        draw_snake();
        erase(tail[tail_end_shift]);

        tail[tail_end_shift] = head_pos;
        tail_end_shift++;
        tail_end_shift %= snake_size;
    }
}