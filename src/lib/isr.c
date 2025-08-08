#include <isr.h>

#include <ports.h>
#include <screen.h>
#include <string.h>

static isr_t interrupt_handlers[IDT_ENTRIES];

void isr_common_handler(uint32_t int_no)
{
    char buffer[3];
    int_to_str(int_no, buffer);

    put_string(0, buffer);

    if (interrupt_handlers[int_no])
        interrupt_handlers[int_no]();

    if (int_no >= 40 && int_no < 48)
        outb(PIC2_COMMAND, PIC_EOI); // slave

    if (int_no >= 32)
        outb(PIC1_COMMAND, PIC_EOI); // master
}

void register_interrupt_handler(uint32_t int_no, isr_t handler)
{
    interrupt_handlers[int_no] = handler;
}
