#include "settings_manager.h"
#include <drivers/keyboard.h>
#include <drivers/screen.h>
#include <kernel/settings.h>
#include <lib/string.h>
#include <lib/math_generic.h>
// #include <drivers/mouse.h>

#define TOP_PAD 5 // For correct display, values less than 3 are not recommended.
#define BOTTOM_PAD 2
#define LEFT_PAD 2
#define RIGHT_PAD 5

#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25

#define OPTIONS_GAP 2
#define OPTIONS_GAP_OFFSET ((OPTIONS_GAP + 2) * SCREEN_WIDTH)

#define OPTIONS_LENGTH (uint8_t)(sizeof(options) / sizeof(options[0]))

#define OPTIONS_IN_COLLUM \
    (((SCREEN_HEIGHT - TOP_PAD - BOTTOM_PAD) + ((2 + OPTIONS_GAP) - 1)) / (2 + OPTIONS_GAP))

#define MAX_PAGES (uint8_t)(OPTIONS_LENGTH / (OPTIONS_IN_COLLUM * 2))

static uint8_t selected_option = 0;
static uint8_t current_page = 0;

typedef enum
{
    SLIDER,
    NUMERIC,
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
            } numeric;
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

static int popup_read_number(uint8_t input_max_len, uint8_t height, uint8_t width, uint8_t y_position)
{
    set_cursor_visibility(true);

    uint16_t buf[width * height];

    for (int i = 0; i < width * height; i++)
    {
        int pos = ((SCREEN_WIDTH * y_position +  // set y_position row
                    SCREEN_WIDTH / 2 -           // set row cencer
                    input_max_len / 2) +         // offset row to left for input center align
                   (i / width * SCREEN_WIDTH) +  // calculate y_position+1 row
                   i % width) -                  // calculate position in y_position+1 row
                  ((width - input_max_len) / 2); // width center align
        buf[i] = get_attrchar(pos);
        set_bg_color(pos, CYAN);
        put_char(pos, 0);
    }
    put_string((SCREEN_WIDTH * (y_position + height / 2 - 1) + SCREEN_WIDTH / 2 - (strlen("New value:") / 2 - 1)), "New value:");
    set_cursor_pos((SCREEN_WIDTH * (y_position + height / 2) + SCREEN_WIDTH / 2 - input_max_len / 2));
    int input = read_number_conf(input_max_len, true);
    for (int i = 0; i < width * height; i++)
    {
        int pos = ((SCREEN_WIDTH * y_position +  // set y_position row
                    SCREEN_WIDTH / 2 -           // set row cencer
                    input_max_len / 2) +         // offset row to left for input center align
                   (i / width * SCREEN_WIDTH) +  // calculate y_position+1 row
                   i % width) -                  // calculate position in y_position+1 row
                  ((width - input_max_len) / 2); // width center align
        put_attrchar(pos, buf[i]);
    }
    set_cursor_visibility(false);
    return input;
}

static void generic_slider_left(option_t *opt)
{
    if (opt->data.value < opt->data.slider.min_value + opt->data.slider.step)
        settings_set_int(opt->meta.key, opt->data.slider.min_value);
    else
        settings_set_int(opt->meta.key, opt->data.value - opt->data.slider.step);
    opt->data.value = settings_get_int(opt->meta.key, 0);
}

static void generic_slider_right(option_t *opt)
{
    if (opt->data.value > opt->data.slider.max_value - opt->data.slider.step)
        settings_set_int(opt->meta.key, opt->data.slider.max_value);
    else
        settings_set_int(opt->meta.key, opt->data.value + opt->data.slider.step);
    opt->data.value = settings_get_int(opt->meta.key, 0);
}

static void generic_checkbox(option_t *opt)
{
    opt->data.value = !opt->data.value;
    settings_set_int(opt->meta.key, opt->data.value);
}

static void generic_numeric(option_t *opt)
{
    int input = popup_read_number(max(num_digits(opt->data.numeric.max_value), num_digits(opt->data.numeric.min_value)), 6, 17, 9);
    opt->data.value = max(min(input, opt->data.numeric.max_value), opt->data.numeric.min_value); // set value in bounds
    settings_set_int(opt->meta.key, opt->data.value);
}

static option_t options[] = {
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

static void draw_option(option_t *opt, uint8_t pos)
{
    uint16_t el_screen_pos;
    if (TOP_PAD + BOTTOM_PAD + (2 + OPTIONS_GAP) * pos < SCREEN_HEIGHT)
        el_screen_pos = (SCREEN_WIDTH * TOP_PAD + LEFT_PAD + (pos * OPTIONS_GAP_OFFSET));
    else if (TOP_PAD + BOTTOM_PAD + (2 + OPTIONS_GAP) * (pos - OPTIONS_IN_COLLUM) < SCREEN_HEIGHT)
        el_screen_pos = (SCREEN_WIDTH * TOP_PAD + (SCREEN_WIDTH / 2 + LEFT_PAD) + ((pos - OPTIONS_IN_COLLUM) * OPTIONS_GAP_OFFSET));
    else
        return;

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
    case NUMERIC:
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

static void highlight_selection(uint8_t pos, uint8_t color)
{
    uint16_t el_screen_pos;
    if (TOP_PAD + BOTTOM_PAD + (2 + OPTIONS_GAP) * pos < SCREEN_HEIGHT)
        el_screen_pos = (SCREEN_WIDTH * TOP_PAD + (pos * OPTIONS_GAP_OFFSET));
    else if (TOP_PAD + BOTTOM_PAD + (2 + OPTIONS_GAP) * (pos - OPTIONS_IN_COLLUM) < SCREEN_HEIGHT)
        el_screen_pos = (SCREEN_WIDTH * TOP_PAD + (SCREEN_WIDTH / 2) + ((pos - OPTIONS_IN_COLLUM) * OPTIONS_GAP_OFFSET));
    else
        return;

    for (int i = LEFT_PAD; i < SCREEN_WIDTH / 2 - RIGHT_PAD + 1; i++)
        set_bg_color(el_screen_pos + i, color);
}

void settings_manager_main(void)
{
    set_cursor_visibility(false);
restart_settings_manager:
    clear_screen();
    put_string(SCREEN_WIDTH * (TOP_PAD / 2 - 1) + SCREEN_WIDTH / 2 - strlen("Settings") / 2, "Settings");
    if (MAX_PAGES > 1)
    {
        put_string(SCREEN_WIDTH * (TOP_PAD / 2) + SCREEN_WIDTH - strlen("Prev/Next Page: [ ]") - RIGHT_PAD, "Prev/Next Page: [ ]");
        uint8_t num_digits_max_pages = num_digits(MAX_PAGES);
        uint8_t num_digits_curr_page = num_digits(current_page + 1);
        char page_str[7 + num_digits_max_pages * 2];

        for (uint8_t i = 5; i < 7 + num_digits_max_pages * 2 - 6 + num_digits_max_pages; i++)
            page_str[i] = '0';

        page_str[0] = 'P';
        page_str[1] = 'a';
        page_str[2] = 'g';
        page_str[3] = 'e';
        page_str[4] = ' ';
        uint_to_str(current_page + 1, &page_str[5 + num_digits_max_pages - num_digits_curr_page]);
        uint_to_str(MAX_PAGES + 1, &page_str[6 + num_digits_max_pages]);
        page_str[5 + num_digits_max_pages] = '/';
        page_str[7 + num_digits_max_pages * 2 - 1] = '\0';

        put_string(SCREEN_WIDTH * (SCREEN_HEIGHT - BOTTOM_PAD) + SCREEN_WIDTH - RIGHT_PAD - strlen(page_str), page_str);
    }
    for (int i = 0; i < ((current_page == MAX_PAGES) ? (OPTIONS_LENGTH % (OPTIONS_IN_COLLUM * 2)) : (OPTIONS_IN_COLLUM * 2)); i++)
    {
        int ind = i + current_page * (OPTIONS_IN_COLLUM * 2);
        options[ind].data.value = settings_get_int(options[ind].meta.key, 0);
        draw_option(&options[ind], i);
    }

    highlight_selection(selected_option, LIGHT_GREY);

    while (true)
    {
        char c;
        while (!(c = get_keyboard_char()))
            asm volatile("hlt");

        option_t *current_opt = &options[selected_option + current_page * (OPTIONS_IN_COLLUM * 2)];
        switch (c)
        {
        case KEY_ESC:
            return;
            break;
        case KEY_DOWN:
            if (selected_option < ((current_page == MAX_PAGES) ? OPTIONS_LENGTH % (OPTIONS_IN_COLLUM * 2) - 1 : OPTIONS_IN_COLLUM * 2 - 1))
            {
                highlight_selection(selected_option, BLACK);
                selected_option++;
                highlight_selection(selected_option, LIGHT_GREY);
            }
            break;
        case KEY_UP:
            if (selected_option > 0)
            {
                highlight_selection(selected_option, BLACK);
                selected_option--;
                highlight_selection(selected_option, LIGHT_GREY);
            }
            break;
        case KEY_LEFT:
            if (current_opt->handler.left)
            {
                current_opt->handler.left(&current_opt);
                draw_option(&current_opt, selected_option);
            }
            break;
        case KEY_RIGHT:
            if (current_opt->handler.right)
            {
                current_opt->handler.right(&current_opt);
                draw_option(&current_opt, selected_option);
            }
            break;
        case ' ':
        case '\n':
            if (current_opt->handler.middle)
            {
                current_opt->handler.middle(&current_opt);
                draw_option(&current_opt, selected_option);
            }
            break;
        case '[':
            if (current_page > 0)
            {
                current_page--;
                selected_option = 0;
                goto restart_settings_manager;
            }
            break;
        case ']':
            if (current_page < MAX_PAGES)
            {
                current_page++;
                selected_option = 0;
                goto restart_settings_manager;
            }
            break;
        }
    }
}