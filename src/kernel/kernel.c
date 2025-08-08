#include <idt.h>
#include <screen.h>
#include <keyboard.h>
#include "../apps/app_selector/app_selector.h"

void kernel_main()
{
    clear_screen();

    idt_install();
    keyboard_install();

    app_selector();
}