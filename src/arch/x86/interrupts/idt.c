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
