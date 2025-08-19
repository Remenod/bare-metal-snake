#include <interrupts/idt.h>
#include <drivers/screen.h>
#include <drivers/keyboard.h>
#include <timer/pit.h>
#include <drivers/vga.h>
#include <lib/mem.h>
#include <lib/string.h>
#include "../../apps/app_selector/app_selector.h"

void kernel_main()
{
    idt_install();
    keyboard_install();
    pit_init(1000);

    app_selector();
}
