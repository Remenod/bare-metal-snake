#include <lib/mem.h>

void *memcpy(void *dest, const void *src, uint32_t n)
{
    uint8_t *d = dest;
    const uint8_t *s = src;
    for (uint32_t i = 0; i < n; i++)
        d[i] = s[i];
    return dest;
}

void *memset(void *dst, int value, unsigned count)
{
    uint8_t *p = dst;
    while (count--)
        *p++ = (uint8_t)value;
    return dst;
}

void pokeb(unsigned seg, unsigned off, uint8_t val)
{
    volatile uint8_t *addr = (volatile uint8_t *)(seg * 16 + off);
    *addr = val;
}

uint8_t peekb(unsigned seg, unsigned off)
{
    volatile uint8_t *addr = (volatile uint8_t *)(seg * 16 + off);
    return *addr;
}

void pokew(unsigned seg, unsigned off, uint16_t val)
{
    volatile uint16_t *ptr = (volatile uint16_t *)(seg * 16 + off);
    *ptr = val;
}

uint16_t peekw(void *addr)
{
    return *((volatile uint16_t *)addr);
}
