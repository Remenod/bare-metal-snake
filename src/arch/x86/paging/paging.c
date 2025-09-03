#include <paging/paging.h>

static size_t page_directory[1024] __attribute__((aligned(4096)));

static size_t page_table0[1024] __attribute__((aligned(4096)));

extern void set_cr_paging_bit(void);

void enable_paging(void)
{
    asm volatile("cli");

    asm volatile("int3");
    set_cr_paging_bit();
    asm volatile("sti");
}