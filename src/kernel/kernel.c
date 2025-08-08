#include <idt.h>
#include <screen.h>
#include <keyboard.h>
#include "../apps/text_sandbox/text_sandbox.h"

void kernel_main()
{
    clear_screen();

    idt_install();
    keyboard_install();

    text_sandbox_main();
}