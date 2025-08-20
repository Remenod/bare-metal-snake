#include <interrupts/isr.h>

#include <ports.h>
#include <interrupts/rsod_routine.h>

static isr_t interrupt_handlers[IDT_ENTRIES];

void isr_common_handler(uint32_t int_no)
{
    if (interrupt_handlers[int_no])
        interrupt_handlers[int_no]();

    if (int_no >= 40 && int_no < 48)
        outb(PIC2_COMMAND, PIC_EOI); // slave

    if (int_no >= 32)
        outb(PIC1_COMMAND, PIC_EOI); // master
}

void isr_exception_handler(uint32_t int_no, uint32_t err_code)
{
    struct cpu_state state;

    asm volatile(
        "mov %%eax, %0\n"
        "mov %%ebx, %1\n"
        "mov %%ecx, %2\n"
        "mov %%edx, %3\n"
        "mov %%esi, %4\n"
        "mov %%edi, %5\n"
        "mov %%ebp, %6\n"
        : "=m"(state.eax), "=m"(state.ebx), "=m"(state.ecx),
          "=m"(state.edx), "=m"(state.esi), "=m"(state.edi),
          "=m"(state.ebp));

    uint32_t esp, ss;
    asm volatile(
        "mov %%esp, %0\n"
        "mov %%ss, %1\n"
        : "=r"(esp), "=r"(ss));

    uint32_t eip = *((uint32_t *)(esp + 0));
    uint32_t cs = *((uint32_t *)(esp + 4));
    uint32_t eflags = *((uint32_t *)(esp + 8));

    state.int_no = int_no;
    state.err_code = err_code;
    state.eip = eip;
    state.cs = cs;
    state.eflags = eflags;
    state.useresp = esp;
    state.ss = ss;

    if (interrupt_handlers[int_no])
        interrupt_handlers[int_no](&state);
}

void register_interrupt_handler(uint32_t int_no, isr_t handler)
{
    interrupt_handlers[int_no] = handler;
}
