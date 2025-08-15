#pragma once

#include <lib/types.h>

typedef struct
{
    uint32_t seed;
} Random;

void random_init(Random *r, uint32_t seed);
uint32_t random_next(Random *r);
uint32_t random_next_bounded(Random *r, uint32_t max);
