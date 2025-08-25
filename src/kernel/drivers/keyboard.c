#include <drivers/keyboard.h>

#include <ports.h>
#include <interrupts/isr.h>
#include <lib/types.h>
#include <drivers/screen.h>

#define KBD_DATA_PORT 0x60

static const char scancode_ascii[128] = {
    0, 27, '1', '2', '3', '4', '5', '6',
    '7', '8', '9', '0', '-', '=', '\b', '\t',
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i',
    'o', 'p', '[', ']', '\n', 0, 'a', 's',
    'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',
    '\'', '`', 0, '\\', 'z', 'x', 'c', 'v',
    'b', 'n', 'm', ',', '.', '/', 0, '*',
    0, ' ', 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0};

static volatile char last_char = 0;
static volatile bool_t extended = 0;
char read_number_buf[12];

void keyboard_handler(void)
{
    uint8_t scancode = inb(KBD_DATA_PORT);

    if (scancode == 0xE0)
    {
        extended = true;
        return;
    }

    if (scancode & 0x80)
    {
        extended = false;
        return;
    }

    if (extended)
    {
        switch (scancode)
        {
        case 0x48:
            last_char = KEY_UP;
            break;
        case 0x50:
            last_char = KEY_DOWN;
            break;
        case 0x4B:
            last_char = KEY_LEFT;
            break;
        case 0x4D:
            last_char = KEY_RIGHT;
            break;
        }
        extended = false;
        return;
    }

    if (scancode < sizeof(scancode_ascii))
    {
        char c = scancode_ascii[scancode];
        if (c)
            last_char = c;
    }
}

char get_char(void)
{
    char c = last_char;
    last_char = 0;
    return c;
}

int read_number(void)
{
    int idx = 0;
    bool_t negative = false;

    while (1)
    {
        char c = 0;
        while (!(c = get_char()))
            asm volatile("hlt");

        if (c == '\n' || c == '\r')
        {
            if (idx == 0)
                continue;
            read_number_buf[idx] = '\0';
            break;
        }

        if (c == '\b' && idx > 0)
        {
            idx--;
            print_char(c);
            continue;
        }

        if (idx == 0 && c == '-')
        {
            negative = true;
            read_number_buf[idx++] = c;
            print_char(c);
            continue;
        }

        if (c >= '0' && c <= '9' && idx < (int)(sizeof(read_number_buf) - 1))
        {
            read_number_buf[idx++] = c;
            print_char(c);
        }
    }

    int value = 0;
    int start = (negative ? 1 : 0);
    for (int i = start; read_number_buf[i] != '\0'; i++)
    {
        value = value * 10 + (read_number_buf[i] - '0');
    }
    if (negative)
        value = -value;

    return value;
}

uint32_t read_hex(void)
{
    int idx = 0;
    bool_t has_0x = false;

    while (1)
    {
        char c = 0;
        while (!(c = get_char()))
            asm volatile("hlt");

        if (c == '\n' || c == '\r')
        {
            if (idx == 0)
                continue;
            read_number_buf[idx] = '\0';
            break;
        }

        if (c == '\b' && idx > 0)
        {
            idx--;
            print_char(c);
            continue;
        }

        if (idx == 0 && c == '0')
        {
            read_number_buf[idx++] = c;
            print_char(c);
            continue;
        }

        if (idx == 1 && read_number_buf[0] == '0' && (c == 'x' || c == 'X'))
        {
            read_number_buf[idx++] = c;
            has_0x = true;
            print_char(c);
            continue;
        }

        if (((c >= '0' && c <= '9') ||
             (c >= 'a' && c <= 'f') ||
             (c >= 'A' && c <= 'F')) &&
            idx < (int)(sizeof(read_number_buf) - 1))
        {
            read_number_buf[idx++] = c;
            print_char(c);
        }
    }

    int value = 0;
    int start = (has_0x ? 2 : 0);
    for (int i = start; read_number_buf[i] != '\0'; i++)
    {
        char c = read_number_buf[i];
        int digit = 0;

        if (c >= '0' && c <= '9')
            digit = c - '0';
        else if (c >= 'a' && c <= 'f')
            digit = 10 + (c - 'a');
        else if (c >= 'A' && c <= 'F')
            digit = 10 + (c - 'A');

        value = (value << 4) | digit;
    }

    return value;
}

void keyboard_install(void)
{
    register_interrupt_handler(33, keyboard_handler);
}
