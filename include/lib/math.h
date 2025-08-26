#pragma once

#include <lib/types.h>

#define abs(val)            \
    _Generic((val),         \
        int8_t: abs_int8,   \
        int16_t: abs_int16, \
        int32_t: abs_int32)(val)

inline int8_t abs_int8(int8_t val)
{
    return val < 0 ? -val : val;
}
inline int16_t abs_int16(int16_t val)
{
    return val < 0 ? -val : val;
}
inline int32_t abs_int32(int32_t val)
{
    return val < 0 ? -val : val;
}

inline uint8_t num_digits(int n)
{
    int digits = 0;
    do
    {
        n /= 10;
        digits++;
    } while (n);
    return digits;
}