#pragma once

#include <lib/types.h>

#define PI 3.14159265f

#define sin(val)     \
    _Generic((val),  \
        float: sinf, \
        double: sind)(val)

#define cos(val)     \
    _Generic((val),  \
        float: cosf, \
        double: cosd)(val)

#define abs(val)            \
    _Generic((val),         \
        int8_t: abs_int8,   \
        int16_t: abs_int16, \
        int32_t: abs_int32)(val)

inline float sinf(float x)
{
    while (x > PI)
        x -= 2.0f * PI;
    while (x < -PI)
        x += 2.0f * PI;

    float x2 = x * x;
    float term1 = x;
    float term2 = (x * x2) / 6.0f;
    float term3 = (x * x2 * x2) / 120.0f;
    float term4 = (x * x2 * x2 * x2) / 5040.0f;

    return term1 - term2 + term3 - term4;
}
inline double sind(double x)
{
    while (x > PI)
        x -= 2.0 * PI;
    while (x < -PI)
        x += 2.0 * PI;

    double x2 = x * x;
    double term1 = x;
    double term2 = (x * x2) / 6.0f;
    double term3 = (x * x2 * x2) / 120.0f;
    double term4 = (x * x2 * x2 * x2) / 5040.0f;

    return term1 - term2 + term3 - term4;
}

inline float cosf(float x)
{
    while (x > PI)
        x -= 2.0f * PI;
    while (x < -PI)
        x += 2.0f * PI;

    float x2 = x * x;
    float term1 = 1.0f;
    float term2 = x2 / 2.0f;
    float term3 = (x2 * x2) / 24.0f;
    float term4 = (x2 * x2 * x2) / 720.0f;

    return term1 - term2 + term3 - term4;
}
inline double cosd(double x)
{
    while (x > PI)
        x -= 2.0f * PI;
    while (x < -PI)
        x += 2.0f * PI;

    double x2 = x * x;
    double term1 = 1.0f;
    double term2 = x2 / 2.0f;
    double term3 = (x2 * x2) / 24.0f;
    double term4 = (x2 * x2 * x2) / 720.0f;

    return term1 - term2 + term3 - term4;
}

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