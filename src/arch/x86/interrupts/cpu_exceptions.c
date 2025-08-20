#include <interrupts/isr.h>
#include <drivers/vga.h>
#include <drivers/screen.h>
#include <lib/types.h>

void show_rsod()
{
    set_text_mode();
    set_cursor_visibility(false);
    //
}

void register_all_cpu_exceptions_isrs()
{
    register_interrupt_handler(0, isr_divide_error);
    register_interrupt_handler(1, isr_debug);
    register_interrupt_handler(2, isr_nmi);
    register_interrupt_handler(3, isr_breakpoint);
    register_interrupt_handler(4, isr_overflow);
    register_interrupt_handler(5, isr_bound_range);
    register_interrupt_handler(6, isr_invalid_opcode);
    register_interrupt_handler(7, isr_device_not_available);
    register_interrupt_handler(8, isr_double_fault);
    register_interrupt_handler(9, isr_coprocessor_segment_overrun);
    register_interrupt_handler(10, isr_invalid_tss);
    register_interrupt_handler(11, isr_segment_not_present);
    register_interrupt_handler(12, isr_stack_segment_fault);
    register_interrupt_handler(13, isr_general_protection);
    register_interrupt_handler(14, isr_page_fault);
    register_interrupt_handler(16, isr_fpu_floating_point);
    register_interrupt_handler(17, isr_alignment_check);
    register_interrupt_handler(18, isr_machine_check);
    register_interrupt_handler(19, isr_simd_floating_point);
    register_interrupt_handler(20, isr_virtualization);
}

/* Divide Error */
void isr_divide_error();

/* Debug Exception */
void isr_debug();

/* Non-Maskable Interrupt (NMI) */
void isr_nmi();

/* Breakpoint (int3) */
void isr_breakpoint();

/* Overflow (into instruction) */
void isr_overflow();

/* Bound Range Exceeded - (bound instruction) */
void isr_bound_range();

/* Invalid Opcode */
void isr_invalid_opcode();

/* Device Not Available - FPU or coprocessor not available */
void isr_device_not_available();

/* Double Fault */
void isr_double_fault(uint32_t err_code);

/* Coprocessor Segment Overrun (Intel only) */
void isr_coprocessor_segment_overrun();

/* Invalid TSS */
void isr_invalid_tss(uint32_t err_code);

/* Segment Not Present */
void isr_segment_not_present(uint32_t err_code);

/* Stack Segment Fault */
void isr_stack_segment_fault(uint32_t err_code);

/* General Protection Fault */
void isr_general_protection(uint32_t err_code);

/* Page Fault */
void isr_page_fault(uint32_t err_code);

/* x87 FPU Floating-Point Error */
void isr_fpu_floating_point();

/* Alignment Check */
void isr_alignment_check(uint32_t err_code);

/* Machine Check - serious hardware error */
void isr_machine_check();

/* SIMD Floating-Point Exception */
void isr_simd_floating_point();

/* Virtualization Exception */
void isr_virtualization();