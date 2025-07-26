#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <keyboard.h>
#include <screen.h>

void kernel_main()
{
    clear_screen();

    idt_install();
    keyboard_install();

    char hello[] = "Hello, world!";

    put_string(80 * 25 / 2 - strlen(hello) / 2, hello);
    int i = 10;
    while (true)
    {
        char c;
        while (!(c = get_char()))
            ;
        put_char(i, c);
        i++;
    }

    asm volatile("hlt");
}