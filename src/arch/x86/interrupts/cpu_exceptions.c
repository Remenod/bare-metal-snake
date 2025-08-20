#include <interrupts/isr.h>
#include <drivers/vga.h>
#include <drivers/screen.h>
#include <lib/types.h>

void show_rsod(const char *msg)
{
    set_text_mode();
    set_cursor_visibility(false);
    fill_screen(0, BLACK, RED);

    print(
        " ___  ___  ___  ___  \n"
        "| _ \\/ __|/ _ \\|   \\ \n"
        "|   /\\__ \\ (_) | |) |\n"
        "|_|_\\|___/\\___/|___/  routine\n\n");

    if (msg)
    {
        print("KERNEL PANIC: ");
        print(msg);
        print("\n");
    }

    for (;;)
        asm volatile("hlt");
}

/* 0 Divide Error */
void isr_divide_error();

/* 1 Debug Exception */
void isr_debug();

/* 2 Non-Maskable Interrupt (NMI) */
void isr_nmi();

/* 3 Breakpoint (int3) */
void isr_breakpoint()
{
    show_rsod("breakpoint");
    asm volatile("cli");
    while (true)
        ;
}

/* 4 Overflow (into instruction) */
void isr_overflow();

/* 5 Bound Range Exceeded - (bound instruction) */
void isr_bound_range();

/* 6 Invalid Opcode */
void isr_invalid_opcode();

/* 7 Device Not Available - FPU or coprocessor not available */
void isr_device_not_available();

/* 8 Double Fault */
void isr_double_fault(uint32_t err_code);

/* 9 Coprocessor Segment Overrun (Intel only) */
void isr_coprocessor_segment_overrun();

/* 10 Invalid TSS */
void isr_invalid_tss(uint32_t err_code);

/* 11 Segment Not Present */
void isr_segment_not_present(uint32_t err_code);

/* 12 Stack Segment Fault */
void isr_stack_segment_fault(uint32_t err_code);

/* 13 General Protection Fault */
void isr_general_protection(uint32_t err_code);

/* 14 Page Fault */
void isr_page_fault(uint32_t err_code);

/* 16 x87 FPU Floating-Point Error */
void isr_fpu_floating_point();

/* 17 Alignment Check */
void isr_alignment_check(uint32_t err_code);

/* 18 Machine Check - serious hardware error */
void isr_machine_check();

/* 19 SIMD Floating-Point Exception */
void isr_simd_floating_point();

/* 20 Virtualization Exception */
void isr_virtualization();

void register_all_cpu_exceptions_isrs()
{
    // register_interrupt_handler(0, isr_divide_error);
    // register_interrupt_handler(1, isr_debug);
    // register_interrupt_handler(2, isr_nmi);
    register_interrupt_handler(3, isr_breakpoint);
    // register_interrupt_handler(4, isr_overflow);
    // register_interrupt_handler(5, isr_bound_range);
    // register_interrupt_handler(6, isr_invalid_opcode);
    // register_interrupt_handler(7, isr_device_not_available);
    // register_interrupt_handler(8, isr_double_fault);
    // register_interrupt_handler(9, isr_coprocessor_segment_overrun);
    // register_interrupt_handler(10, isr_invalid_tss);
    // register_interrupt_handler(11, isr_segment_not_present);
    // register_interrupt_handler(12, isr_stack_segment_fault);
    // register_interrupt_handler(13, isr_general_protection);
    // register_interrupt_handler(14, isr_page_fault);
    // register_interrupt_handler(16, isr_fpu_floating_point);
    // register_interrupt_handler(17, isr_alignment_check);
    // register_interrupt_handler(18, isr_machine_check);
    // register_interrupt_handler(19, isr_simd_floating_point);
    // register_interrupt_handler(20, isr_virtualization);
}