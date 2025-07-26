#include <stdint.h>
#include <stddef.h>
#include <string.h>

void kernel_main()
{
    clear_screen();
    char hello[] = "Hello, world!";

    put_string(80 * 25 / 2 - strlen(hello) / 2, hello);
    while (1)
        ;
}