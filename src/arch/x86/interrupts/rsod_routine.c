#include <interrupts/rsod_routine.h>

#include <ports.h>
#include <drivers/vga.h>
#include <drivers/screen.h>

#define MAX_RSOD_LOG 15
#define RSOD_MSG_LEN 35

static const char *rsod_log[MAX_RSOD_LOG];
static int rsod_log_count = 0;

void rsod_add_log(const char *msg)
{
    static char truncated_msg[MAX_RSOD_LOG][RSOD_MSG_LEN + 1];

    if (rsod_log_count < MAX_RSOD_LOG)
    {
        int i;
        for (i = 0; i < RSOD_MSG_LEN && msg[i] != '\0'; i++)
            truncated_msg[rsod_log_count][i] = msg[i];
        truncated_msg[rsod_log_count][i] = '\0';

        rsod_log[rsod_log_count] = truncated_msg[rsod_log_count];
        rsod_log_count++;
    }
}

void show_rsod(const char *msg, struct cpu_state *state)
{
    set_text_mode();
    set_cursor_visibility(false);
    fill_screen(0, BLACK, RED);

    print(
        "  ___  ___  ___  ___  \n"
        " | _ \\/ __|/ _ \\|   \\ \n"
        " |   /\\__ \\ (_) | |) |\n"
        " |_|_\\|___/\\___/|___/  r o u t i n e\n\n");

    if (msg)
    {
        print(" KERNEL PANIC: ");
        print(msg);
        print("\n Int number: ");
        print_dec(state->int_no);
        print(" Error code: ");
        print_hex(state->err_code);
        print("\n\n");
        print(
            " A critical error has occurred\n"
            " System halted to prevent data\n"
            " corruption.\n\n");
    }

    for (int i = 0; i < rsod_log_count; i++)
    {
        put_string(40 + (80 * i), "LOG:");
        put_string(45 + (80 * i), rsod_log[i]);
    }

    if (state)
    {
        print(" CPU STATE:\n");
        print(" EIP: ");
        print_hex(state->eip);
        print("  CS:  ");
        print_hex(state->cs);
        print("\n");
        print(" ESP: ");
        print_hex(state->useresp);
        print("  SS:  ");
        print_hex(state->ss);
        print("\n");
        print(" EFLAGS: ");
        print_hex(state->eflags);
        print("\n");
        print(" EAX: ");
        print_hex(state->eax);
        print("  EBX: ");
        print_hex(state->ebx);
        print("  ECX: ");
        print_hex(state->ecx);
        print("  EDX: ");
        print_hex(state->edx);
        print("\n");
        print(" ESI: ");
        print_hex(state->esi);
        print("  EDI: ");
        print_hex(state->edi);
        print("  EBP: ");
        print_hex(state->ebp);
        print("\n\n");

        if (state->int_no == 0x0E) // CR2 page fault (0x0E)
        {
            uint32_t cr2;
            asm volatile("mov %%cr2, %0" : "=r"(cr2));
            print(" CR2 (Paging fault addr): ");
            print_hex(cr2);
            print("\n");
        }
    }

    while (true)
        asm volatile("hlt");
}
