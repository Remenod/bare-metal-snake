#include <interrupts/stack_guard/stack_guard.h>

#include <timer/pit.h>
#include <interrupts/rsod_routine.h>
#include <drivers/screen.h>
#include <lib/string.h>
#include <lib/math.h>

extern void stack_guard_warning(void);
extern void stack_guard_panic(void);

static uint16_t stack_guard_warning_buf[80];

static char buf[12];

void stack_guard_warning_routine(void)
{
    pop_pit_task();
    register_pit_task(stack_guard_panic);

    for (int i = 0; i < 80; i++)
    {
        stack_guard_warning_buf[i] = get_attrchar(i);
        put_char(i, 0);
        set_bg_color(i, YELLOW);
        set_fg_color(i, BLACK);
    }

    put_string(1, "WARNING! Stack is about to overflow! Remaining capacity 25%  Resume in:");

    for (int i = 3999999, i_len = num_digits(i), curr_i_len; i > 0; i--)
    {
        curr_i_len = num_digits(i);
        put_string(73 + (i_len - curr_i_len), uint_to_str(i, buf));
        if (curr_i_len != num_digits(i - 1))
            put_char(73 + (i_len - curr_i_len), '0');
    }

    for (int i = 0; i < 80; i++)
        put_attrchar(i, stack_guard_warning_buf[i]);
}

void cancel_panic_ready_routine(void)
{
    pop_pit_task();
    register_pit_task(stack_guard_warning);
}

void stack_guard_install(void)
{
    register_pit_task(stack_guard_warning);
}