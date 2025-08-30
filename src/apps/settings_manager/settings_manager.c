#include "settings_manager.h"
#include <drivers/keyboard.h>
#include <drivers/screen.h>
#include <kernel/settings.h>
#include <lib/string.h>
#include <lib/math.h>
// #include <drivers/mouse.h>

#define LEFT_PAD 2
#define RIGHT_PAD 5

#define SCREEN_WIDTH 80

#define OPTIONS_GAP 1
#define OPTIONS_GAP_OFFSET ((OPTIONS_GAP + 2) * SCREEN_WIDTH)

#define OPTIONS_SIZE (uint8_t)(sizeof(options) / sizeof(options[0]))

uint8_t selected_option = 0;

typedef enum
{
    SLIDER,
    NUMBER,
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

void generic_checkbox(option_t *opt)
{
    opt->data.value = !opt->data.value;
    settings_set_int(opt->meta.key, opt->data.value);
}

void generic_number(option_t *opt)
{
    set_vga_cursor_visibility(true);
    int input;
    {
        const uint8_t input_max_len = 11, height = 4, width = 15;

        uint16_t buf[width * height];

        for (int i = 0; i < width * height; i++)
        {
            int pos = ((SCREEN_WIDTH * 10 + SCREEN_WIDTH / 2 - input_max_len / 2) + (i / width * SCREEN_WIDTH) + i % width) - 2;
            buf[i] = get_attrchar(pos);
            set_bg_color(pos, CYAN);
        }
        put_string(SCREEN_WIDTH * 11 + SCREEN_WIDTH / 2 - (strlen("New value:") / 2 - 1), "New value:");
        set_vga_cursor_pos(SCREEN_WIDTH * 12 + SCREEN_WIDTH / 2 - input_max_len / 2);
        input = read_number();
        for (int i = 0; i < width * height; i++)
        {
            int pos = ((SCREEN_WIDTH * 10 + SCREEN_WIDTH / 2 - input_max_len / 2) + (i / width * SCREEN_WIDTH) + i % width) - 2;
            put_attrchar(pos, buf[i]);
        }
    }
    opt->data.value = max_int(min_int(input, opt->data.number.max_value), opt->data.number.min_value); // set value in bounds
    settings_set_int(opt->meta.key, opt->data.value);
    set_vga_cursor_visibility(false);
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
                .min_value = 20,
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
    uint16_t el_screen_pos = (SCREEN_WIDTH * 4 + LEFT_PAD + (pos * OPTIONS_GAP_OFFSET));
    put_string(el_screen_pos - SCREEN_WIDTH, opt->meta.caption);
    switch (opt->meta.type)
    {
    case SLIDER:
    {
        opt->data.value = max_int(min_int(opt->data.value, opt->data.slider.max_value), opt->data.slider.min_value); // set value in bounds

        int slider_len = (opt->data.slider.max_value - opt->data.slider.min_value) / opt->data.slider.step + 1;
        if (slider_len > SCREEN_WIDTH / 2 - LEFT_PAD - RIGHT_PAD)
        {
            put_string(el_screen_pos, "[Slider too long]");
            return;
        }
        put_char(el_screen_pos, 0xC6);
        for (int i = 1; i < slider_len + 1; i++)
            put_char(el_screen_pos + i, 0xCD);
        put_char(el_screen_pos + slider_len + 1, 0xB5);
        put_char(el_screen_pos + 1 + ((opt->data.value - opt->data.slider.min_value) / opt->data.slider.step), 0xD8);
        break;
    }
    case CHECKBOX:
        if (opt->data.value)
            put_string(el_screen_pos, "ON  \xDD\xDF\xDE");
        else
            put_string(el_screen_pos, "OFF \xDD\xDC\xDE");
        break;
    case NUMBER:
        for (uint8_t i = 0; i < SCREEN_WIDTH / 2 - RIGHT_PAD + 1; i++)
            put_char(el_screen_pos + i, 0);

        put_char(el_screen_pos, '[');
        char buf[12];
        put_string(el_screen_pos + 2, int_to_str(opt->data.value, buf));
        put_char(el_screen_pos + num_digits(opt->data.value) + 3, ']');
        break;

    default:
        break;
    }
}

void settings_manager_main(void)
{
    set_vga_cursor_visibility(false);
    put_string(SCREEN_WIDTH * 3 / 2 - strlen("Settings") / 2, "Settings");
    for (int i = 0; i < OPTIONS_SIZE; i++)
    {
        options[i].data.value = settings_get_int(options[i].meta.key, 0);
        draw_option(&options[i], i);
    }

    {
        uint16_t el_screen_pos = (SCREEN_WIDTH * 4 + (selected_option * OPTIONS_GAP_OFFSET));
        for (int i = LEFT_PAD; i < SCREEN_WIDTH / 2 - RIGHT_PAD + 1; i++)
            set_bg_color(el_screen_pos + i, LIGHT_GREY);
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
            if (selected_option < OPTIONS_SIZE - 1)
            {
                uint16_t el_screen_pos = (SCREEN_WIDTH * 4 + (selected_option * OPTIONS_GAP_OFFSET));
                for (int i = LEFT_PAD; i < SCREEN_WIDTH / 2 - RIGHT_PAD + 1; i++)
                {
                    set_bg_color(el_screen_pos + i, BLACK);
                    set_bg_color(el_screen_pos + i + OPTIONS_GAP_OFFSET, LIGHT_GREY);
                }
                selected_option++;
            }
            break;
        case KEY_UP:
            if (selected_option > 0)
            {
                uint16_t el_screen_pos = (80 * 4 + (selected_option * OPTIONS_GAP_OFFSET));
                for (int i = LEFT_PAD; i < SCREEN_WIDTH / 2 - RIGHT_PAD + 1; i++)
                {
                    set_bg_color(el_screen_pos + i - OPTIONS_GAP_OFFSET, LIGHT_GREY);
                    set_bg_color(el_screen_pos + i, BLACK);
                }
                selected_option--;
            }
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