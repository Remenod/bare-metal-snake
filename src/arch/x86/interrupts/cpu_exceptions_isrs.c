#include <interrupts/isr.h>
#include <interrupts/rsod_routine.h>

#define DEFINE_UNSPECIAL_ISR(n, msg)                 \
    _Noreturn void isr_##n(const cpu_state_t *state) \
    {                                                \
        show_rsod(msg, state);                       \
        __builtin_unreachable();                     \
    }

// add implemented isr vec here
#define CPU_EXCEPTIONS X(0) X(1) X(3) X(4) X(6) X(8) X(13)

DEFINE_UNSPECIAL_ISR(0, "Division by Zero");
DEFINE_UNSPECIAL_ISR(1, "Debug");
/* 2 Non-Maskable Interrupt (NMI) */
DEFINE_UNSPECIAL_ISR(4, "Signed Int Overflow");
/* 5 Bound Range Exceeded - (bound instruction) */
DEFINE_UNSPECIAL_ISR(6, "Invalid Opcode");
/* 7 Device Not Available - FPU or coprocessor not available */
DEFINE_UNSPECIAL_ISR(8, "Double Fault");
/* 9 Coprocessor Segment Overrun (Intel only) */
DEFINE_UNSPECIAL_ISR(13, "General Protection Fault");
/* 10 Invalid TSS */
/* 11 Segment Not Present */
/* 12 Stack Segment Fault */
/* 14 Page Fault */
/* 16 x87 FPU Floating-Point Error */
/* 17 Alignment Check */
/* 18 Machine Check - serious hardware error */
/* 19 SIMD Floating-Point Exception */
/* 20 Virtualization Exception */

/* Breakpoint (int3) */
_Noreturn void isr_3() // marked as stateless in cpu_interrupts.asm
{
    asm volatile("cli");
    while (true)
        ;
}

void register_all_cpu_exceptions_isrs()
{
#define X(n) register_interrupt_handler(n, isr_##n);
    CPU_EXCEPTIONS
#undef X
}