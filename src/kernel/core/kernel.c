#include <interrupts/idt.h>
#include <drivers/screen.h>
#include <drivers/keyboard.h>
#include <drivers/mouse.h>
#include <timer/pit.h>
#include <drivers/vga.h>
#include <interrupts/cpu_exceptions.h>
#include <interrupts/stack_guard/stack_guard.h>
#include "../../apps/app_selector/app_selector.h"

void kernel_main()
{
    idt_install();
    pit_init(1000);
    register_all_cpu_exceptions_isrs();
    keyboard_install();
    mouse_install();
    stack_guard_install();

    set_graphics_mode();
    draw_mode13h_test_pattern();
    set_text_mode();

    clear_screen();

    app_selector();
}
