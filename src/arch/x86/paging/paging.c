#include <paging/paging.h>
#include <paging/page_table.h>
#include <paging/page_directory.h>

static pde_t page_directory[1024] __attribute__((aligned(4096)));
static pte_t page_table0[1024] __attribute__((aligned(4096)));

extern void load_page_directory(pde_t page_dir[1024]);
extern void enable_global_pages(void);
extern void enable_paging(void);

static void setup_identity_mapping(void)
{
    for (uint32_t i = 0; i < 1024; i++)
        pte_init(&page_table0[i], i * 0x1000, 1, 0, 0, 0, 0, 0, 0);

    pde_init(&page_directory[0], (uint32_t)page_table0, 1, 0, 0, 0, 0, 0);

    for (uint32_t i = 1; i < 1024; i++)
    {
        page_directory[i].raw.data = 0;
        pde_set_present_flag(&page_directory[i], 0);
    }
}

void paging_init(void)
{
    asm volatile("cli");
    setup_identity_mapping();
    load_page_directory(page_directory);
    enable_global_pages();
    enable_paging();
    asm volatile("sti");
}