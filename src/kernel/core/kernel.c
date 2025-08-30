#include <interrupts/idt.h>
#include <drivers/screen.h>
#include <drivers/keyboard.h>
#include <timer/pit.h>
#include <drivers/vga.h>
#include <interrupts/cpu_exceptions.h>
#include <kernel/diagnostics/stack_guard/stack_guard.h>
#include <kernel/diagnostics/warning_routine.h>
#include <kernel/settings.h>
#include "../../apps/app_selector/app_selector.h"

void kernel_main()
{
    idt_install();
    pit_init(1000);
    register_all_cpu_exceptions_isrs();
    calibrate_warning_iter_per_tick();
    keyboard_install();
    stack_guard_install();

    settings_init();

    set_graphics_mode();
    draw_mode13h_test_pattern();
    set_text_mode();

    clear_screen();

    app_selector();
}
