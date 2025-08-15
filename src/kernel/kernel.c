#include <interrupts/idt.h>
#include <drivers/screen.h>
#include <drivers/keyboard.h>
#include <interrupts/pit.h>
#include "../apps/app_selector/app_selector.h"

void kernel_main()
{
    clear_screen();

    idt_install();
    keyboard_install();

    pit_init(1000);

    app_selector();
}