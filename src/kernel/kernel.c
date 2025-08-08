#include <idt.h>
#include <screen.h>
#include <keyboard.h>
#include <pit.h>
#include "../apps/app_selector/app_selector.h"

void kernel_main()
{
    clear_screen();

    idt_install();
    keyboard_install();

    pit_init(1000);

    app_selector();
}