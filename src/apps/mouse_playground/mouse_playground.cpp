#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

#include "mouse_playground.h"
extern "C"
{
#include <drivers/mouse.h>
#include <drivers/screen.h>
#include <drivers/keyboard.h>
#include <lib/string.h>
}

static const char m1_text[] = " Great! Now mouse2 ";
static const char m2_text[] = "Impressive! Mouse3?";
static const char m3_text[] = " Hey! Click, here! ";

void mouse_playground_main(void)
{
    set_vga_cursor_visibility(false);
    put_string(80 * 25 / 2 - strlen(m3_text) / 2, m3_text);

    register_ui_element(0, (mouse_ui_element_t){
                               .bound = [](uint16_t x, uint16_t y) -> bool_t
                               {
                                   return x > 270 && x < 380 && y > 188 && y < 210;
                               },
                               .mouse1_handler = [](uint16_t x, uint16_t y) -> void
                               {
                                   put_string(80 * 25 / 2 - strlen(m2_text) / 2, m1_text);
                               },
                               .mouse2_handler = [](uint16_t x, uint16_t y) -> void
                               {
                                   put_string(80 * 25 / 2 - strlen(m1_text) / 2, m2_text);
                               },
                               .mouse3_handler = [](uint16_t x, uint16_t y) -> void
                               {
                                   put_string(80 * 25 / 2 - strlen(m1_text) / 2, m3_text);
                               },
                           });
    while (true)
    {
        char c;
        while (!(c = get_keyboard_char()))
            asm volatile("hlt");
        if (c == KEY_ESC)
        {
            reset_ui_structure();
            break;
        }
    }
}
#pragma GCC diagnostic pop