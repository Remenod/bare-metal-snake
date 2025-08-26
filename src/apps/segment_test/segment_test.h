#pragma once

#include <lib/types.h>

typedef struct
{
    uint32_t base;
    uint32_t limit;
    uint32_t access;
} seg_desc_t;

typedef struct __attribute__((packed))
{
    uint16_t limit;
    uint32_t base;
} gdtr_t;

void segment_test_main(void);