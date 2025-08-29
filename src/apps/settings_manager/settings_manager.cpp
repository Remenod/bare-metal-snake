#include "settings_manager.h"
extern "C"
{
#include <drivers/keyboard.h>
#include <drivers/screen.h>
#include <drivers/qemu_serial.h>
#include <kernel/settings.h>
#include <lib/string.h>
#include <lib/math.h>
    // #include <drivers/mouse.h>
}

#define LEFT_PAD 2
#define OPTIONS_SIZE (uint8_t)(sizeof(options) / sizeof(options[0]))

uint8_t selected_option = 0;

typedef enum
{
    SLIDER,
    VALUE,
    CHECKBOX
} option_type_t;

struct option; // forward declaration

typedef void (*option_handler_t)(struct option *opt);

typedef struct option
{
    struct
    {
        const char *caption;
        const char *key;
        option_type_t type;
    } meta;
    struct
    {
        int value;
        union
        {
            struct
            {
                int min_value;
                int max_value;
                int step;
            } slider;
            struct
            {
                int min_value;
                int max_value;
            } number;
            struct
            {
                // currently not needed
            } checkbox;
        };
    } data;
    struct
    {
        option_handler_t left;
        option_handler_t right;
        option_handler_t middle;
    } handler;
} option_t;

void generic_slider_left(option_t *opt)
{
    if (opt->data.value < opt->data.slider.min_value + opt->data.slider.step)
        settings_set_int(opt->meta.key, opt->data.slider.min_value);
    else
        settings_set_int(opt->meta.key, opt->data.value - opt->data.slider.step);
    opt->data.value = settings_get_int(opt->meta.key, 0);
}
void generic_slider_right(option_t *opt)
{
    if (opt->data.value > opt->data.slider.max_value - opt->data.slider.step)
        settings_set_int(opt->meta.key, opt->data.slider.max_value);
    else
        settings_set_int(opt->meta.key, opt->data.value + opt->data.slider.step);
    opt->data.value = settings_get_int(opt->meta.key, 0);
}

option_t options[] = {
    {
        .meta = {
            .caption = "Mouse sensitivity",
            .key = "mouse.sensitivity",
            .type = SLIDER,
        },
        .data = {
            .value = 0,
            .slider = {
                .min_value = 1,
                .max_value = 250,
                .step = 10,
            },
        },
        .handler = {
            .left = generic_slider_left,
            .right = generic_slider_right,
            .middle = (option_handler_t)NULL,
        },
    },
};

void draw_option(option_t *opt, uint8_t pos)
{
    uint16_t el_screen_pos = (80 * 4 + LEFT_PAD + (pos * 160));
    put_string(el_screen_pos - 80, opt->meta.caption);
    switch (opt->meta.type)
    {
    case SLIDER:
    {
        opt->data.value = max_int(min_int(opt->data.value, opt->data.slider.max_value), opt->data.slider.min_value); // set value in bounds

        int slider_len = (opt->data.slider.max_value - opt->data.slider.min_value) / opt->data.slider.step;
        if (slider_len > 40 - LEFT_PAD)
        {
            put_string(el_screen_pos, "[Slider too long]");
            return;
        }
        put_char(el_screen_pos, '<');
        for (int i = 1; i < slider_len + 1; i++)
            put_char(el_screen_pos + i, '=');
        put_char(el_screen_pos + slider_len + 1, '>');
        put_char(el_screen_pos + 1 + ((opt->data.value - opt->data.slider.min_value) / opt->data.slider.step), 219);
        break;
    }
    case CHECKBOX:
        /* TODO */
        break;
    case VALUE:
        /* TODO */
        break;

    default:
        break;
    }
}

void settings_manager_main(void)
{
    set_vga_cursor_visibility(false);
    put_string(80 * 3 / 2 - strlen("Settings") / 2, "Settings");
    for (int i = 0; i < OPTIONS_SIZE; i++)
    {
        serial_write_uint32(options[i].data.value);
        options[i].data.value = settings_get_int(options[i].meta.key, 0);
        serial_write_uint32(options[i].data.value);
        draw_option(&options[i], i);
    }
    while (true)
    {
        char c;
        while (!(c = get_keyboard_char()))
            asm volatile("hlt");
        switch (c)
        {
        case KEY_ESC:
            return;
            break;
        case KEY_DOWN:
            if (selected_option < OPTIONS_SIZE)
                selected_option++;
            break;
        case KEY_UP:
            if (selected_option > 0)
                selected_option--;
            break;
        case KEY_LEFT:
            if (options[selected_option].handler.left)
            {
                options[selected_option].handler.left(&options[selected_option]);
                draw_option(&options[selected_option], selected_option);
            }
            break;
        case KEY_RIGHT:
            if (options[selected_option].handler.right)
            {
                options[selected_option].handler.right(&options[selected_option]);
                draw_option(&options[selected_option], selected_option);
            }
            break;
        case ' ':
        case '\n':
            if (options[selected_option].handler.middle)
            {
                options[selected_option].handler.middle(&options[selected_option]);
                draw_option(&options[selected_option], selected_option);
            }
            break;
        }
    }
}