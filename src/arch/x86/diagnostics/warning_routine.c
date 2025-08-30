#include <kernel/diagnostics/warning_routine.h>

#include <drivers/screen.h>
#include <lib/math.h>
#include <lib/string.h>

static uint16_t cover_buf[80];
static char buf[12];

void kernel_warning(const char *msg, uint32_t duration, bool_t manage_irq)
{
    if (manage_irq)
        asm volatile("cli");
    for (int i = 0; i < 80; i++)
    {
        cover_buf[i] = get_attrchar(i);
        put_char(i, 0);
        set_bg_color(i, YELLOW);
        set_fg_color(i, BLACK);
    }

    put_string(1, "WARNING!");
    put_string(10, msg);
    put_string(62, "Resume in:");

    for (int i = duration, i_len = num_digits(i), curr_i_len; i > 0; i--)
    {
        curr_i_len = num_digits(i);
        put_string(73 + (i_len - curr_i_len), uint_to_str(i, buf));
        if (curr_i_len != num_digits(i - 1))
            put_char(73 + (i_len - curr_i_len), '0');
    }

    for (int i = 0; i < 80; i++)
        put_attrchar(i, cover_buf[i]);
    if (manage_irq)
        asm volatile("sti");
}