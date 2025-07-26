#include <stdint.h>
#include <string.h>
#include <stdbool.h>

void kernel_main()
{
    clear_screen();
    char hello[] = "Hello, world!";

    put_string(80 * 25 / 2 - strlen(hello) / 2, hello);
    put_char(0, '1');
    while (true)
        asm volatile("hlt");
}