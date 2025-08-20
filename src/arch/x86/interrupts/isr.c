#include <interrupts/isr.h>

#include <ports.h>

static isr_t interrupt_handlers[IDT_ENTRIES];

void isr_common_handler(uint32_t int_no, uint32_t err_code)
{
    char buffer[3];
    if (int_no != 32)
        put_string(1998, int_to_str(int_no, buffer));
    if (interrupt_handlers[int_no])
        interrupt_handlers[int_no](err_code);

    if (int_no >= 40 && int_no < 48)
        outb(PIC2_COMMAND, PIC_EOI); // slave

    if (int_no >= 32)
        outb(PIC1_COMMAND, PIC_EOI); // master
}

void register_interrupt_handler(uint32_t int_no, isr_t handler)
{
    interrupt_handlers[int_no] = handler;
}
