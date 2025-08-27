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
    const char done_text[] = "Done\n";
    print("\nInstalling IDT... ");
    idt_install();
    print(done_text);
    print("PIT Initialization... ");
    pit_init(1000);
    print(done_text);
    print("CPU int registration... ");
    register_all_cpu_exceptions_isrs();
    print(done_text);
    print("Installing keyboard... ");
    keyboard_install();
    print(done_text);
    print("Installing mouse... ");
    mouse_install();
    print(done_text);
    print("Installing Stack Guard... ");
    stack_guard_install();
    print(done_text);

    print("Testing VGA modes... ");
    set_graphics_mode();
    draw_mode13h_test_pattern();
    set_text_mode();
    print(done_text);

    clear_screen();

    app_selector();
}
