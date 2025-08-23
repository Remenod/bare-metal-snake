#include <lib/random.h>

void random_init(Random *r, uint32_t seed)
{
    r->seed = seed;
}

uint32_t random_next(Random *r)
{
    r->seed ^= r->seed << 13;
    r->seed ^= r->seed >> 17;
    r->seed ^= r->seed << 5;
    return r->seed;
}

uint32_t random_next_bounded(Random *r, uint32_t max)
{
    return random_next(r) % max;
}

uint32_t random_next_range(Random *r, uint32_t min, uint32_t max)
{
    if (max <= min)
        return min;

    return min + (random_next(r) % (max - min + 1));
}
