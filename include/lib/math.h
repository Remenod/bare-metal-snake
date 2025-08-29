#pragma once

#include <lib/types.h>

#define PI 3.14159265358979323846
#define E 2.71828182845904523536
#define LN2 0.69314718055994530942
#define LN10 2.30258509299404568402
#define SQRT2 1.41421356237309504880

#define PIF 3.14159265f
#define EF 2.71828182f
#define LN2F 0.69314718f
#define LN10F 2.30258509f
#define SQRT2F 1.41421356f

#define DEG2RAD(deg) ((deg) * (PI / 180.0))
#define RAD2DEG(rad) ((rad) * (180.0 / PI))

float fmodf(float x, float y);
double fmodd(double x, double y);

float expf(float x);
double expd(double x);

float sinf(float x);
double sind(double x);

float cosf(float x);
double cosd(double x);

float tanf(float x);
double tand(double x);

float ctgf(float x);
double ctgd(double x);

float asinf(float x);
double asind(double x);

float acosf(float x);
double acosd(double x);

float atanf(float x);
double atand(double x);

float actgf(float x);
double actgd(double x);

float sinhf(float x);
double sinhd(double x);

float coshf(float x);
double coshd(double x);

float tanhf(float x);
double tanhd(double x);

float sqrtf(float x);
double sqrtd(double x);

float lnf(float x);
double lnd(double x);

float log10f(float x);
double log10d(double x);

float powf(float base, float exp);
double powd(double base, double exp);

float logbf(float x, float base);
double logbd(double x, double base);

int32_t pow_int32(int32_t base, int32_t exp);
uint32_t pow_uint32(uint32_t base, uint32_t exp);
int64_t pow_int64(int64_t base, int64_t exp);
uint64_t pow_uint64(uint64_t base, uint64_t exp);

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
inline int64_t abs_int64(int64_t val)
{
    return val < 0 ? -val : val;
}
inline float absf(float val)
{
    return val < 0 ? -val : val;
}
inline double absd(double val)
{
    return val < 0 ? -val : val;
}

inline float floorf(float x)
{
    int xi = (int)x;
    return (x < 0 && x != xi) ? xi - 1 : xi;
}
inline double floord(double x)
{
    long xi = (long)x;
    return (x < 0 && x != xi) ? xi - 1 : xi;
}

inline float ceilf(float x)
{
    int xi = (int)x;
    return (x > 0 && x != xi) ? xi + 1 : xi;
}
inline double ceild(double x)
{
    long xi = (long)x;
    return (x > 0 && x != xi) ? xi + 1 : xi;
}

inline float roundf(float x)
{
    return (x >= 0) ? floorf(x + 0.5f) : ceilf(x - 0.5f);
}
inline double roundd(double x)
{
    return (x >= 0) ? floord(x + 0.5) : ceild(x - 0.5);
}

inline int min_int(int a, int b) { return a < b ? a : b; }
inline int max_int(int a, int b) { return a > b ? a : b; }

inline float minf(float a, float b) { return a < b ? a : b; }
inline double mind(double a, double b) { return a < b ? a : b; }

inline float maxf(float a, float b) { return a > b ? a : b; }
inline double maxd(double a, double b) { return a > b ? a : b; }

inline int mod(int a, int b) { return ((a % b) + b) % b; }

inline uint8_t num_digits(int n)
{
    uint8_t digits = 0;
    if (n < 0)
    {
        digits++;
        n = abs_int32(n);
    }
    do
    {
        n /= 10;
        digits++;
    } while (n);
    return digits;
}
