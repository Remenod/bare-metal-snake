#include "segment_test.h"

#include <drivers/keyboard.h>
#include <drivers/screen.h>
#include <lib/string.h>

static char buf[12];

static const char *const seg_info_text[] = {
    "Base:   0x",
    "Limit:  0x",
    "Access: 0x",
    "Segment Info",
};

static uint16_t tested_segment;
static uint8_t (*read_addr)(uint32_t addr);

extern uint8_t read_addr_via_cs(uint32_t addr);
extern uint8_t read_addr_via_ds(uint32_t addr);
extern uint8_t read_addr_via_ss(uint32_t addr);
extern uint8_t read_addr_via_es(uint32_t addr);
extern uint8_t read_addr_via_fs(uint32_t addr);
extern uint8_t read_addr_via_gs(uint32_t addr);

static const uint8_t (*const read_addr_arr[])(uint32_t addr) = {
    read_addr_via_cs,
    read_addr_via_ds,
    read_addr_via_ss,
    read_addr_via_es,
    read_addr_via_fs,
    read_addr_via_gs,
};

static seg_desc_t get_segment_info(uint16_t selector)
{
    seg_desc_t info = {0};

    uint32_t limit;
    asm volatile("lsl %1, %0" : "=r"(limit) : "r"(selector));
    info.limit = limit;

    uint32_t access;
    asm volatile("lar %1, %0" : "=r"(access) : "r"(selector));
    info.access = access;

    gdtr_t gdtr;
    asm volatile("sgdt %0" : "=m"(gdtr));

    uint32_t index = selector >> 3;
    uint64_t *gdt = (uint64_t *)gdtr.base;
    uint64_t desc = gdt[index];

    uint32_t base = ((desc >> 16) & 0xFFFFFF) | ((desc >> 56) & 0xFF) << 24;
    info.base = base;

    return info;
}

static void print_seg_info(seg_desc_t *info)
{
    put_string(80 - 25, "0x");
    put_string(80 - 23, uint_to_str_hex(tested_segment, &buf));
    put_string(80 - 20, seg_info_text[3]);
    for (int i = 0; i < 3; i++)
    {
        put_string((80 * (i + 2)) - 25, seg_info_text[i]);
        put_string((80 * (i + 2)) - 15, uint_to_str_hex(((uint32_t *)info)[i], &buf));
    }
}

/* 0 - cs, 1 - ds, 2 - ss, 3 - es, 4 - fs, 5 - gs */
static uint16_t get_seg_selector(uint8_t seg)
{
    uint16_t sel = 0;
    switch (seg)
    {
    case 0:
        asm volatile("mov %%cs, %0" : "=r"(sel));
        break;
    case 1:
        asm volatile("mov %%ds, %0" : "=r"(sel));
        break;
    case 2:
        asm volatile("mov %%ss, %0" : "=r"(sel));
        break;
    case 3:
        asm volatile("mov %%es, %0" : "=r"(sel));
        break;
    case 4:
        asm volatile("mov %%fs, %0" : "=r"(sel));
        break;
    case 5:
        asm volatile("mov %%gs, %0" : "=r"(sel));
        break;
    }
    return sel;
}

void segment_test_main(void)
{
    seg_desc_t seg_info;

    static const char segs[] = {'c', 'd', 's', 'e', 'f', 'g'};
    while (true)
    {
        clear_screen();
        print("Choose segment:\n");

        for (uint16_t i = 0, seg_code; i < 6; i++)
        {
            print_dec(i + 1);
            print(". ");
            print_char(segs[i]);
            print("s ");
            print_hex(get_seg_selector(i));
            print_char('\n');
        }
        print("7. Custom segment selector from GDT\n> ");

        uint8_t choice = read_number();
        if (choice > 0 && choice < 7)
        {
            tested_segment = get_seg_selector(choice - 1);
            print_char('\n');
            read_addr = read_addr_arr[choice - 1];
            break;
        }
        else if (choice == 7)
        {
            print("\nEnter your selector(hex): ");
            tested_segment = read_hex();
            if (tested_segment % 8)
            {
                tested_segment -= tested_segment % 8;
                print("\nSelector correction. Selected: ");
                print_hex(tested_segment);
                print_char('\n');
            }
            asm volatile(
                "mov %0, %%gs"
                :
                : "r"(tested_segment)
                : "memory");
            read_addr = read_addr_via_gs;
            break;
        }
        else
        {
            print("\nInvalid selection!\n");
            sleep(1000);
        }
    }

    uint32_t addr;
    uint16_t cursor_pos_buf;

    print("Enter an address offset(hex) to read byte from.\nBe careful cuz u might get an RSoD btw\n");

    while (true)
    {
        seg_info = get_segment_info(tested_segment);
        print_seg_info(&seg_info);

        cursor_pos_buf = get_cursor_pos();
        static const char wanna_continue_text[] = "Press any key to enable typing or ESC to leave";
        print(wanna_continue_text);
        for (int i = cursor_pos_buf, len = strlen(wanna_continue_text); i < len + cursor_pos_buf; i++)
            set_fg_color(i, DARK_GREY);
        set_cursor_pos(cursor_pos_buf);

        char c;
        while (!(c = get_keyboard_char()))
            asm volatile("hlt");

        switch (c)
        {
        case KEY_ESC:
            return;
            break;
        default:
            for (int i = cursor_pos_buf, len = strlen(wanna_continue_text); i < len + cursor_pos_buf; i++)
            {
                put_char(i, 0);
                set_fg_color(i, WHITE);
            }
            addr = read_hex();
            print_char('(');
            print_udec(addr);
            print_char(')');
            print_char(' ');
            print_hex(read_addr(addr));
            print_char('\n');
            if (get_cursor_pos() > 1840)
            {
                for (int i = 1; i < 5; i++)
                    for (int j = 80 * i - 25; j < 80 * i; j++)
                        put_char(j, ' ');
                scroll_down();
                print_seg_info(&seg_info);
            }
        }
    }
}