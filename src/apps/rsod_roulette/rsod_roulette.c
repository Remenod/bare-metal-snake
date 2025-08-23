#include "rsod_roulette.h"

#include <drivers/qemu_serial.h>
#include <interrupts/isr.h>

#include <lib/random.h>
#include <timer/pit.h>
#include <drivers/screen.h>
#include <lib/string.h>
#include <lib/math.h>

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
        "movw $0x2235, %%ax\n"
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

static Random rand;

static const Crash crashes[] = {
    {"Division by Zero", div_by_zero},
    {"Protection Fault", gp},
    {"Invalid Opcode", inv_opcode},
    {"Segment Not Present", seg_np},
    {"Bound Range Excess", bound_exc},
    {"Overflow Exception", sig_overflow},
};

static inline void put_text_in_center(const char *msg, int8_t place)
{
    const uint8_t colors[] = {WHITE, LIGHT_GREY, DARK_GREY, BLACK};
    uint16_t msg_len = strlen(msg);
    uint16_t start_pos = (80 * 25 / 2 - msg_len / 2) + place * 3 * 80;

    uint8_t color =
        place
            ? colors[abs(place) < (COLORS_LEN - 1)
                         ? (abs(place))
                         : (COLORS_LEN - 1)]
            : WHITE;

    for (int i = 0; i < 80; i++)
        put_char(start_pos - start_pos % 80 + i, 0);

    for (uint16_t i = start_pos; i < start_pos + msg_len; i++)
        set_fg_color(i, color);

    if (!place)
        put_char(start_pos - 2, '>');

    put_string(start_pos, msg);

    if (!place)
        put_char(start_pos + strlen(msg) + 1, '<');
}

static inline const Crash *spin_crashes(void)
{
    float motion = 1.0f;
    uint8_t res = 0;

    for (uint32_t i = 0; (uint32_t)motion < random_next_range(&rand, 400, 1200); i++)
    {
        for (int8_t j = -2; j < 3; j++)
        {
            res = (i + j) % CRASHES_LEN;
            put_text_in_center(crashes[res].msg, j);
        }

        motion *= 1.05f;

        sleep((uint32_t)motion);
    }
    for (int i = 0; i < 80; i++)
        set_fg_color(80 * 12 + i, GREEN);
    return &crashes[(res + CRASHES_LEN - 2) % CRASHES_LEN];
}

void rsod_roulette_main(void)
{
    set_cursor_visibility(false);

    if (!rand.seed)
        random_init(&rand, get_timer_ticks() + get_timer_ticks());

    const Crash *choice = spin_crashes();

    put_char(0, '3');
    sleep(1000);
    put_char(0, '2');
    sleep(1000);
    put_char(0, '1');
    sleep(1000);

    choice->crash();

    while (true)
        ;
}