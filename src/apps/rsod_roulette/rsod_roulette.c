#include "rsod_roulette.h"

#include <lib/random.h>
#include <timer/pit.h>
#include <drivers/screen.h>
#include <drivers/keyboard.h>
#include <lib/string.h>
#include <lib/math.h>
#include <interrupts/rsod_routine.h>

#define CRASHES_LEN (uint8_t)(sizeof(crashes) / sizeof(crashes[0]))
#define COLORS_LEN (uint8_t)(sizeof(colors) / sizeof(colors[0]))

typedef struct
{
    const char *msg;
    func_t crash;
} Crash;

static void div_by_zero(void)
{
    volatile int a = 0x1230;
    volatile int b = 0;
    volatile int c;
    c = a / b;
    a = c;
}

static void gp(void)
{
    asm volatile("int $100");
}

static void inv_opcode(void)
{
    asm volatile("ud2");
}

static void seg_np(void)
{
    asm volatile(
        "movw $0x18, %%ax\n"
        "movw %%ax, %%ds\n" : : : "ax");
}

static void bound_exc(void)
{
    int arr[2] = {0, 10};
    int val = 20;

    asm volatile(
        "bound %[v], %[a]"
        :
        : [v] "r"(val), [a] "m"(arr));
}

static void sig_overflow(void)
{
    volatile uint8_t a1 = 127;
    volatile uint8_t b1 = 1;
    asm volatile(
        "movb %0, %%al\n\t"
        "addb %1, %%al\n\t"
        "into\n\t"
        :
        : "r"(a1), "r"(b1)
        : "al");
}

static const char spin_art[4][21] = {
    " ___ ___ ___ _  _ _ ",
    "/ __| _ \\_ _| \\| | |",
    "\\__ \\  _/| || .` |_|",
    "|___/_| |___|_|\\_(_)"};

static const char countdown_3_art[4][6] = {
    " ____",
    "|__ /",
    " |_ \\",
    "|___/"};

static const char countdown_2_art[4][6] = {
    " ___ ",
    "|_  )",
    " / / ",
    "/___|"};

static const char countdown_1_art[4][4] = {
    " _ ",
    "/ |",
    "| |",
    "|_|"};

static const char launch_text[] = "Press SPACE to continue!";

static Random rand;

static const Crash crashes[] = {
    {"Division by Zero", div_by_zero},
    {"Protection Fault", gp},
    {"Invalid Opcode", inv_opcode},
    {"Segment Not Present", seg_np},
    {"Bound Range Excess", bound_exc},
    {"Overflow Exception", sig_overflow},
};

static void put_art_text(const char *art, uint8_t lines)
{
    uint8_t line_len = strlen(art);
    uint16_t start_pos = ((80 - line_len) / 2) + 80;

    for (int i = 0; i < lines; i++)
    {
        for (int j = 0; j < 80; j++)
            put_char((start_pos - start_pos % 80) + j + 80 * i, 0); // erase whole line

        put_string(start_pos + 80 * i, art + (line_len + 1) * i);
    }
}

static inline void put_roulette_text(const char *msg, int8_t place)
{
    static const uint8_t colors[] = {WHITE, LIGHT_GREY, DARK_GREY, BLACK};
    uint16_t msg_len = strlen(msg);
    uint16_t start_pos = (80 * 25 / 2 - msg_len / 2) + place * 3 * 80 + 80 * 3;

    uint8_t color =
        place
            ? colors[abs(place) < (COLORS_LEN - 1)
                         ? (abs(place))
                         : (COLORS_LEN - 1)]
            : WHITE;

    for (int i = 0; i < 80; i++)
        put_char(start_pos - start_pos % 80 + i, 0); // erase whole line

    for (uint16_t i = start_pos; i < start_pos + msg_len; i++)
        set_fg_color(i, color);

    put_string(start_pos, msg);

    if (!place)
    {
        put_char(start_pos - 2, '>');
        put_char(start_pos + msg_len + 1, '<');
    }
}

static inline const Crash *spin_crashes(void)
{
    float motion = 1.0f;
    uint8_t res = 0;

    for (uint32_t i = 0; (uint32_t)motion < random_next_range(&rand, 400, 1200); i++)
    {
        for (int8_t j = -2; j < 3; j++)
        {
            int idx = (int)(i + j) % (int)CRASHES_LEN;
            if (idx < 0)
                idx += CRASHES_LEN;
            res = (uint8_t)idx;
            put_roulette_text(crashes[res].msg, j);
        }

        motion *= 1.05f;

        sleep((uint32_t)motion);
    }
    for (int i = 0; i < 80; i++)
        set_fg_color(80 * 12 + i + 80 * 3, GREEN);

    return &crashes[(res + CRASHES_LEN - 2) % CRASHES_LEN];
}

static inline void countdown(void)
{
    put_art_text(&countdown_3_art, 4);
    sleep(1000);
    put_art_text(&countdown_2_art, 4);
    sleep(1000);
    put_art_text(&countdown_1_art, 4);
    sleep(1000);
}

void rsod_roulette_main(void)
{
    set_cursor_visibility(false);

    put_art_text(&spin_art, 4);

    put_string(((80 - strlen(launch_text)) / 2) + 80 * 12, launch_text); // Add launch_text

    while (true)
    {
        char c;
        while (!(c = get_char()))
            asm volatile("hlt");
        switch (c)
        {
        case KEY_ESC:
            return;
            break;
        case ' ':
            goto SPIN;
            break;
        case '\n':
            random_init(&rand, get_timer_ticks() + get_timer_ticks());
            crashes[random_next_bounded(&rand, CRASHES_LEN)].crash();
            break;
        }
    }

SPIN:
    for (int j = 0; j < 80; j++) // Remove launch_text
        put_char(80 * 12 + j, 0);

    if (!rand.seed)
        random_init(&rand, get_timer_ticks() + get_timer_ticks());

    const Crash *choice = spin_crashes();

    countdown();

    rsod_add_log("Hello from RSoD Coundown!");
    choice->crash();
}