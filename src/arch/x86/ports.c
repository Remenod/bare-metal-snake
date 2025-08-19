#include <ports.h>

uint8_t inb(uint16_t port)
{
    uint8_t ret;
    asm volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}
void outb(uint16_t port, uint8_t data)
{
    asm volatile("outb %0, %1" : : "a"(data), "Nd"(port));
}

uint16_t inw(uint16_t port)
{
    uint16_t ret;
    asm volatile("inw %w1, %w0" : "=a"(ret) : "Nd"(port));
    return ret;
}
void outw(uint16_t port, uint16_t data)
{
    asm volatile("outw %w0, %w1" : : "a"(data), "Nd"(port));
}

uint32_t inl(uint16_t port)
{
    uint32_t ret;
    asm volatile("inl %w1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}
void outl(uint16_t port, uint32_t data)
{
    asm volatile("outl %0, %w1" : : "a"(data), "Nd"(port));
}
