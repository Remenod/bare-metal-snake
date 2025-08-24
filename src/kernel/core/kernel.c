#include <interrupts/idt.h>
#include <drivers/screen.h>
#include <drivers/keyboard.h>
#include <timer/pit.h>
#include <drivers/vga.h>
#include <interrupts/cpu_exceptions.h>
#include <timer/stack_guard.h>
#include "../../apps/app_selector/app_selector.h"

void kernel_main()
{
    idt_install();
    pit_init(1000);
    stack_guard_install();
    register_all_cpu_exceptions_isrs();
    keyboard_install();

    set_graphics_mode();
    draw_mode13h_test_pattern();
    set_text_mode();

    clear_screen();

    app_selector();
}
