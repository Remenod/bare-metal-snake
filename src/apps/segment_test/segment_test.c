#include "segment_test.h"

#include <drivers/keyboard.h>
#include <drivers/screen.h>

extern uint8_t test_gs_addres_acces(uint32_t addr);

void segment_test_main(void)
{
    uint32_t addr;
    while (true)
    {
        char c;
        while (!(c = get_char()))
            asm volatile("hlt");
        switch (c)
        {
        case KEY_ESC:
            return;
            break;
        default:
            addr = read_hex();
            print_char(' ');
            print_hex(test_gs_addres_acces(addr));
            print_char('\n');
        }
    }
}