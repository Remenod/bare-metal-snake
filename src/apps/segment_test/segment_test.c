#include "segment_test.h"

#include <drivers/keyboard.h>
#include <drivers/screen.h>

extern uint8_t read_arrd_via_gs(uint32_t addr);

void segment_test_main(void)
{
    uint32_t addr;
    uint32_t accesses = 0;

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
            while (true)
            {
                addr = read_hex();
                print_char('(');
                print_udec(addr);
                print_char(')');
                print_char(' ');
                print_hex(read_arrd_via_gs(addr));
                print_char('\n');
                if (++accesses > 23)
                    scroll_down();
            }
        }
    }
}