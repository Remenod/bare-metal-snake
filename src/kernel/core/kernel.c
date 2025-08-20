#include <interrupts/idt.h>
#include <drivers/screen.h>
#include <drivers/keyboard.h>
#include <timer/pit.h>
#include <drivers/vga.h>
#include <interrupts/cpu_exceptions.h>
#include "../../apps/app_selector/app_selector.h"

void kernel_main()
{
    clear_screen();

    idt_install();
    register_all_cpu_exceptions_isrs();
    keyboard_install();
    pit_init(1000);

    // set_graphics_mode();
    // draw_mode13h_test_pattern();
    // set_text_mode();

    app_selector();
}
