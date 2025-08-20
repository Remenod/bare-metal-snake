#include <interrupts/rsod_routine.h>

#include <ports.h>
#include <drivers/vga.h>
#include <drivers/screen.h>

#define MAX_RSOD_LOG 16

static const char *rsod_log[MAX_RSOD_LOG];
static int rsod_log_count = 0;

void rsod_add_log(const char *msg)
{
    if (rsod_log_count < MAX_RSOD_LOG)
        rsod_log[rsod_log_count++] = msg;
}

void show_rsod(const char *msg, struct cpu_state *state)
{
    set_text_mode();
    set_cursor_visibility(false);
    fill_screen(0, BLACK, RED);

    print(
        " ___  ___  ___  ___  \n"
        "| _ \\/ __|/ _ \\|   \\ \n"
        "|   /\\__ \\ (_) | |) |\n"
        "|_|_\\|___/\\___/|___/  r o u t i n e\n\n");

    if (msg)
    {
        print("KERNEL PANIC: ");
        print(msg);
        print("\n\n");
    }

    for (int i = 0; i < rsod_log_count; i++)
    {
        print("LOG: ");
        print(rsod_log[i]);
        print("\n");
    }

    if (state)
    {
        print("\nCPU STATE:\n");
        print("EIP: 0x");
        print_hex(state->eip);
        print("  CS: 0x");
        print_hex(state->cs);
        print("\n");
        print("EFLAGS: 0x");
        print_hex(state->eflags);
        print("\n");
        print("ESP: 0x");
        print_hex(state->useresp);
        print("  SS: 0x");
        print_hex(state->ss);
        print("\n");

        // CR2 page fault (0x0E)
        if (state->int_no == 0x0E)
        {
            uint32_t cr2;
            asm volatile("mov %%cr2, %0" : "=r"(cr2));
            print("CR2 (fault addr): 0x");
            print_hex(cr2);
            print("\n");
        }
    }

    for (;;)
        asm volatile("hlt");
}
