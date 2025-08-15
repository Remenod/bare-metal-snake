#include <lib/mem.h>

#include <lib/types.h>

void *memcpy(void *dest, const void *src, unsigned int n)
{
    uint8_t *d = dest;
    const uint8_t *s = src;
    for (uint32_t i = 0; i < n; i++)
        d[i] = s[i];
    return dest;
}
