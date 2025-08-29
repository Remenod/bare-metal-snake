#include "settings_manager.h"
extern "C"
{
#include <drivers/keyboard.h>
    // #include <drivers/mouse.h>
}

#define OPTIONS_COUNT 2
uint8_t selected_option = 0;

func_t option_handlers[OPTIONS_COUNT][2];

void settings_manager_main(void)
{
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
            if (selected_option < OPTIONS_COUNT)
                selected_option++;
            break;
        case KEY_UP:
            if (selected_option > 0)
                selected_option--;
            break;
        case KEY_LEFT:
            option_handlers[selected_option][0]();
            break;
        case KEY_RIGHT:
            option_handlers[selected_option][1]();
            break;
        }
    }
}