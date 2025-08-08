#include <keyboard.h>

#include <ports.h>
#include <isr.h>
#include <stdbool.h>
#include <stdint.h>

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

void keyboard_handler()
{
    outb(0x20, 0x20); // EOI for PIC1

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

char get_char()
{
    char c = last_char;
    last_char = 0;
    return c;
}

void keyboard_install()
{
    register_interrupt_handler(33, keyboard_handler);
}
