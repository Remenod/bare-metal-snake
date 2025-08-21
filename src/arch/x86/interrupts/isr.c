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

void isr_exception_handler(
    uint32_t gs,
    uint32_t fs,
    uint32_t es,
    uint32_t ds,
    uint32_t edi,
    uint32_t esi,
    uint32_t ebp,
    uint32_t esp,
    uint32_t ebx,
    uint32_t edx,
    uint32_t ecx,
    uint32_t eax,
    uint32_t int_no,
    uint32_t err_code,
    uint32_t eip,
    uint32_t cs,
    uint32_t eflags) // TODO somehow pack this directrly in cpu_interrupts.asm
{
    struct cpu_state state;
    state.eip = eip;
    state.cs = cs;
    state.eflags = eflags;
    state.err_code = err_code;
    state.int_no = int_no;

    state.eax = eax;
    state.ecx = ecx;
    state.edx = edx;
    state.ebx = ebx;
    state.esp = esp;
    state.ebp = ebp;
    state.esi = esi;
    state.edi = edi;

    state.ds = ds;
    state.es = es;
    state.fs = fs;
    state.gs = gs;

    if (interrupt_handlers[int_no])
        interrupt_handlers[int_no](&state);
}
void isr_stateless_exception_handler(
    uint32_t int_no,
    uint32_t eip,
    uint32_t cs,
    uint32_t eflags)
{
    struct cpu_state state = {0};
    state.eip = eip;
    state.cs = cs;
    state.eflags = eflags;
    state.int_no = int_no;

    if (interrupt_handlers[int_no])
        interrupt_handlers[int_no](&state);
}

void register_interrupt_handler(uint32_t int_no, isr_t handler)
{
    interrupt_handlers[int_no] = handler;
}
