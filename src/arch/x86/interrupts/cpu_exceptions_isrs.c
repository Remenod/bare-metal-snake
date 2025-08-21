#include <interrupts/isr.h>
#include <drivers/vga.h>
#include <drivers/screen.h>
#include <lib/types.h>
#include <interrupts/rsod_routine.h>

/* 0 Divide Error */
void isr_divide_error(struct cpu_state *state)
{
    show_rsod("Division by Zero", state);
    asm volatile("cli");
    while (true)
        ;
}

/* 1 Debug Exception */
void isr_debug(struct cpu_state *state);

/* 2 Non-Maskable Interrupt (NMI) */
void isr_nmi(struct cpu_state *state);

/* 3 Breakpoint (int3) */
void isr_breakpoint(struct cpu_state *state) // marked as stateless in cpu_interrupts.asm
{
    asm volatile("cli");
    while (true)
        ;
}

/* 4 Overflow (into instruction) */
void isr_overflow(struct cpu_state *state)
{
    show_rsod("Signed Int Overflow", state);
    asm volatile("cli");
    while (true)
        ;
}

/* 5 Bound Range Exceeded - (bound instruction) */
void isr_bound_range(struct cpu_state *state);

/* 6 Invalid Opcode */
void isr_invalid_opcode(struct cpu_state *state)
{
    show_rsod("Invalid Opcode", state);
    asm volatile("cli");
    while (true)
        ;
}

/* 7 Device Not Available - FPU or coprocessor not available */
void isr_device_not_available(struct cpu_state *state);

/* 8 Double Fault */
void isr_double_fault(struct cpu_state *state)
{
    show_rsod("Double Fault", state);
    asm volatile("cli");
    while (true)
        ;
}

/* 9 Coprocessor Segment Overrun (Intel only) */
void isr_coprocessor_segment_overrun(struct cpu_state *state);

/* 10 Invalid TSS */
void isr_invalid_tss(struct cpu_state *state);

/* 11 Segment Not Present */
void isr_segment_not_present(struct cpu_state *state);

/* 12 Stack Segment Fault */
void isr_stack_segment_fault(struct cpu_state *state);

/* 13 General Protection Fault */
void isr_general_protection(struct cpu_state *state)
{
    show_rsod("General Protection Fault", state);
    asm volatile("cli");
    while (true)
        ;
}

/* 14 Page Fault */
void isr_page_fault(struct cpu_state *state);

/* 16 x87 FPU Floating-Point Error */
void isr_fpu_floating_point(struct cpu_state *state);

/* 17 Alignment Check */
void isr_alignment_check(struct cpu_state *state);

/* 18 Machine Check - serious hardware error */
void isr_machine_check(struct cpu_state *state);

/* 19 SIMD Floating-Point Exception */
void isr_simd_floating_point(struct cpu_state *state);

/* 20 Virtualization Exception */
void isr_virtualization(struct cpu_state *state);

void register_all_cpu_exceptions_isrs()
{
    register_interrupt_handler(0, isr_divide_error);
    // register_interrupt_handler(1, isr_debug);
    // register_interrupt_handler(2, isr_nmi);
    register_interrupt_handler(3, isr_breakpoint);
    register_interrupt_handler(4, isr_overflow);
    // register_interrupt_handler(5, isr_bound_range);
    register_interrupt_handler(6, isr_invalid_opcode);
    // register_interrupt_handler(7, isr_device_not_available);
    register_interrupt_handler(8, isr_double_fault);
    // register_interrupt_handler(9, isr_coprocessor_segment_overrun);
    // register_interrupt_handler(10, isr_invalid_tss);
    // register_interrupt_handler(11, isr_segment_not_present);
    // register_interrupt_handler(12, isr_stack_segment_fault);
    register_interrupt_handler(13, isr_general_protection);
    // register_interrupt_handler(14, isr_page_fault);
    // register_interrupt_handler(16, isr_fpu_floating_point);
    // register_interrupt_handler(17, isr_alignment_check);
    // register_interrupt_handler(18, isr_machine_check);
    // register_interrupt_handler(19, isr_simd_floating_point);
    // register_interrupt_handler(20, isr_virtualization);
}