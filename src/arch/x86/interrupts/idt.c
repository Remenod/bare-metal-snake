#include <interrupts/idt.h>

#include <ports.h>
#include <interrupts/pic.h>

struct idt_entry
{
    uint16_t base_low;
    uint16_t sel;
    uint8_t always0;
    uint8_t flags;
    uint16_t base_high;
} __attribute__((packed));

struct idt_ptr
{
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

extern void isr0();  // Divide-by-zero Error
extern void isr1();  // Debug Exception
extern void isr2();  // Non-Maskable Interrupt (NMI)
extern void isr3();  // Breakpoint Exception
extern void isr4();  // Overflow Exception
extern void isr5();  // Bound Range Exceeded Exception
extern void isr6();  // Invalid Opcode Exception
extern void isr7();  // Device Not Available (FPU) Exception
extern void isr8();  // Double Fault
extern void isr9();  // Coprocessor Segment Overrun (obsolete)
extern void isr10(); // Invalid TSS
extern void isr11(); // Segment Not Present
extern void isr12(); // Stack-Segment Fault
extern void isr13(); // General Protection Fault
extern void isr14(); // Page Fault
extern void isr15(); // Reserved
extern void isr16(); // x87 FPU Floating-Point Error
extern void isr17(); // Alignment Check
extern void isr18(); // Machine Check
extern void isr19(); // SIMD Floating-Point Exception
extern void isr20(); // Virtualization Exception
extern void isr21(); // Reserved
extern void isr22(); // Reserved
extern void isr23(); // Reserved
extern void isr24(); // Reserved
extern void isr25(); // Reserved
extern void isr26(); // Reserved
extern void isr27(); // Reserved
extern void isr28(); // Reserved
extern void isr29(); // Reserved
extern void isr30(); // Security Exception
extern void isr31(); // Reserved
extern void isr32(); // IRQ0: timer
extern void isr33(); // IRQ1: keyboard
extern void isr34(); // IRQ2: cascade
extern void isr35(); // IRQ3: COM2
extern void isr36(); // IRQ4: COM1
extern void isr37(); // IRQ5: LPT2
extern void isr38(); // IRQ6: floppy
extern void isr39(); // IRQ7: LPT1
extern void isr40(); // IRQ8: CMOS
extern void isr41(); // IRQ9
extern void isr42(); // IRQ10
extern void isr43(); // IRQ11
extern void isr44(); // IRQ12: PS/2 mouse
extern void isr45(); // IRQ13: FPU
extern void isr46(); // IRQ14: Primary ATA
extern void isr47(); // IRQ15: Secondary ATA

static struct idt_entry idt[IDT_ENTRIES];
static struct idt_ptr idtp;

void idt_set_gate(int num, uint32_t base, uint16_t sel, uint8_t flags)
{
    idt[num].base_low = base & 0xFFFF;
    idt[num].base_high = (base >> 16) & 0xFFFF;
    idt[num].sel = sel;
    idt[num].always0 = 0;
    idt[num].flags = flags;
}

extern void lidt(void *); // asm-func

void idt_install()
{
    remap_pic();

    idtp.limit = sizeof(struct idt_entry) * IDT_ENTRIES - 1;
    idtp.base = (uint32_t)&idt;

    idt_set_gate(0x00, (uint32_t)isr0, 0x08, 0x8E);
    idt_set_gate(0x01, (uint32_t)isr1, 0x08, 0x8E);
    idt_set_gate(0x02, (uint32_t)isr2, 0x08, 0x8E);
    idt_set_gate(0x03, (uint32_t)isr3, 0x08, 0x8E);
    idt_set_gate(0x04, (uint32_t)isr4, 0x08, 0x8E);
    idt_set_gate(0x05, (uint32_t)isr5, 0x08, 0x8E);
    idt_set_gate(0x06, (uint32_t)isr6, 0x08, 0x8E);
    idt_set_gate(0x07, (uint32_t)isr7, 0x08, 0x8E);
    idt_set_gate(0x08, (uint32_t)isr8, 0x08, 0x8E);
    idt_set_gate(0x09, (uint32_t)isr9, 0x08, 0x8E);
    idt_set_gate(0x0A, (uint32_t)isr10, 0x08, 0x8E);
    idt_set_gate(0x0B, (uint32_t)isr11, 0x08, 0x8E);
    idt_set_gate(0x0C, (uint32_t)isr12, 0x08, 0x8E);
    idt_set_gate(0x0D, (uint32_t)isr13, 0x08, 0x8E);
    idt_set_gate(0x0E, (uint32_t)isr14, 0x08, 0x8E);
    idt_set_gate(0x0F, (uint32_t)isr15, 0x08, 0x8E);
    idt_set_gate(0x10, (uint32_t)isr16, 0x08, 0x8E);
    idt_set_gate(0x11, (uint32_t)isr17, 0x08, 0x8E);
    idt_set_gate(0x12, (uint32_t)isr18, 0x08, 0x8E);
    idt_set_gate(0x13, (uint32_t)isr19, 0x08, 0x8E);
    idt_set_gate(0x14, (uint32_t)isr20, 0x08, 0x8E);
    idt_set_gate(0x15, (uint32_t)isr21, 0x08, 0x8E);
    idt_set_gate(0x16, (uint32_t)isr22, 0x08, 0x8E);
    idt_set_gate(0x17, (uint32_t)isr23, 0x08, 0x8E);
    idt_set_gate(0x18, (uint32_t)isr24, 0x08, 0x8E);
    idt_set_gate(0x19, (uint32_t)isr25, 0x08, 0x8E);
    idt_set_gate(0x1A, (uint32_t)isr26, 0x08, 0x8E);
    idt_set_gate(0x1B, (uint32_t)isr27, 0x08, 0x8E);
    idt_set_gate(0x1C, (uint32_t)isr28, 0x08, 0x8E);
    idt_set_gate(0x1D, (uint32_t)isr29, 0x08, 0x8E);
    idt_set_gate(0x1E, (uint32_t)isr30, 0x08, 0x8E);
    idt_set_gate(0x1F, (uint32_t)isr31, 0x08, 0x8E);
    idt_set_gate(0x20, (uint32_t)isr32, 0x08, 0x8E); // IRQ0
    idt_set_gate(0x21, (uint32_t)isr33, 0x08, 0x8E); // IRQ1
    idt_set_gate(0x22, (uint32_t)isr34, 0x08, 0x8E);
    idt_set_gate(0x23, (uint32_t)isr35, 0x08, 0x8E);
    idt_set_gate(0x24, (uint32_t)isr36, 0x08, 0x8E);
    idt_set_gate(0x25, (uint32_t)isr37, 0x08, 0x8E);
    idt_set_gate(0x26, (uint32_t)isr38, 0x08, 0x8E);
    idt_set_gate(0x27, (uint32_t)isr39, 0x08, 0x8E);
    idt_set_gate(0x28, (uint32_t)isr40, 0x08, 0x8E);
    idt_set_gate(0x29, (uint32_t)isr41, 0x08, 0x8E);
    idt_set_gate(0x2A, (uint32_t)isr42, 0x08, 0x8E);
    idt_set_gate(0x2B, (uint32_t)isr43, 0x08, 0x8E);
    idt_set_gate(0x2C, (uint32_t)isr44, 0x08, 0x8E);
    idt_set_gate(0x2D, (uint32_t)isr45, 0x08, 0x8E);
    idt_set_gate(0x2E, (uint32_t)isr46, 0x08, 0x8E);
    idt_set_gate(0x2F, (uint32_t)isr47, 0x08, 0x8E);

    lidt(&idtp);
    asm volatile("sti");
}
