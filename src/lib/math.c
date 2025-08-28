#include <lib/math.h>

float fmodf(float x, float y)
{
    if (y == 0.0f)
        return 0.0f / 0.0f; // NaN
    int sign = x < 0.0f ? -1 : 1;
    float ax = x < 0.0f ? -x : x;
    float ay = y < 0.0f ? -y : y;
    int32_t n = (int32_t)(ax / ay);
    return sign * (ax - (float)n * ay);
}
double fmodd(double x, double y)
{
    if (y == 0.0)
        return 0.0 / 0.0; // NaN
    int sign = x < 0.0 ? -1 : 1;
    double ax = x < 0.0 ? -x : x;
    double ay = y < 0.0 ? -y : y;
    int64_t n = (int64_t)(ax / ay);
    return sign * (ax - (double)n * ay);
}

/* Basic trigonometry funcs */

float sinf(float x)
{
    x = fmod(x, 2.0f * PIF);
    float x2 = x * x;
    return x * (1 - x2 / 6.0f + x2 * x2 / 120.0f - x2 * x2 * x2 / 5040.0f);
}
double sind(double x)
{
    x = fmod(x, 2.0 * PI);
    double x2 = x * x;
    return x * (1 - x2 / 6.0 + x2 * x2 / 120.0 - x2 * x2 * x2 / 5040.0);
}

float cosf(float x)
{
    x = fmod(x, 2.0f * PIF);
    float x2 = x * x;
    return 1 - x2 / 2 + x2 * x2 / 24 - x2 * x2 * x2 / 720.0f;
}
double cosd(double x)
{
    x = fmod(x, 2.0 * PI);
    double x2 = x * x;
    return 1 - x2 / 2 + x2 * x2 / 24 - x2 * x2 * x2 / 720.0;
}

float tanf(float x) { return sinf(x) / cosf(x); }
double tand(double x) { return sind(x) / cosd(x); }

float ctgf(float x) { return cosf(x) / sinf(x); }
double ctgd(double x) { return cosd(x) / sind(x); }

/* Reverce trigonometry funcs */

float asinf(float x) { return x + (x * x * x) / 6.0f + (3 * x * x * x * x * x) / 40.0f; }
double asind(double x) { return x + (x * x * x) / 6.0 + (3 * x * x * x * x * x) / 40.0; }

float acosf(float x) { return PIF / 2 - asinf(x); }
double acosd(double x) { return PI / 2 - asind(x); }

float atanf(float x) { return x - x * x * x / 3.0f + x * x * x * x * x / 5.0f; }
double atand(double x) { return x - x * x * x / 3.0 + x * x * x * x * x / 5.0; }

float actgf(float x) { return PIF / 2 - atanf(x); }
double actgd(double x) { return PI / 2 - atand(x); }

/* Hyperbolic trigonometry funcs */

float sinhf(float x) { return (expf(x) - expf(-x)) / 2; }
double sinhd(double x) { return (expd(x) - expd(-x)) / 2; }

float coshf(float x) { return (expf(x) + expf(-x)) / 2; }
double coshd(double x) { return (expd(x) + expd(-x)) / 2; }

float tanhf(float x)
{
    float e2 = expf(2 * x);
    return (e2 - 1) / (e2 + 1);
}
double tanhd(double x)
{
    double e2 = expd(2 * x);
    return (e2 - 1) / (e2 + 1);
}

/* Logarithm funcs */

float lnf(float x)
{
    if (x <= 0.0f)
        return 0.0f;

    int k = 0;
    while (x > 1.5f)
    {
        x *= 0.5f;
        k++;
    }
    while (x < 0.5f)
    {
        x *= 2.0f;
        k--;
    }

    x = x - 1.0f;
    float result = 0.0f;
    float term = x;
    for (int i = 1; i <= 20; i++)
    {
        float t = term / i;
        if (i % 2 == 0)
            result -= t;
        else
            result += t;
        term *= x;
    }

    result += k * LN2F;
    return result;
}
double lnd(double x)
{
    if (x <= 0.0)
        return 0.0;

    int k = 0;
    while (x > 1.5)
    {
        x *= 0.5;
        k++;
    }
    while (x < 0.5)
    {
        x *= 2.0;
        k--;
    }

    x = x - 1.0;
    double result = 0.0;
    double term = x;
    for (int i = 1; i <= 30; i++)
    {
        double t = term / i;
        if (i % 2 == 0)
            result -= t;
        else
            result += t;
        term *= x;
    }

    result += k * LN2;
    return result;
}

float log10f(float x)
{
    return lnf(x) / LN10F;
}
double log10d(double x)
{
    return lnd(x) / LN10;
}

float logbf(float x, float base) { return lnf(x) / lnf(base); }
double logbd(double x, double base) { return lnd(x) / lnd(base); }

/* Power functions */

float expf(float x)
{
    if (x == 0.0f)
        return 1.0f;

    int neg = x < 0.0f ? 1 : 0;
    if (neg)
        x = -x;

    float term = 1.0f;
    float sum = 1.0f;
    for (int n = 1; n < 20; n++)
    {
        term *= x / n;
        sum += term;
        if (term < 1e-6f)
            break;
    }

    return neg ? 1.0f / sum : sum;
}
double expd(double x)
{
    if (x == 0.0)
        return 1.0;

    int neg = x < 0.0 ? 1 : 0;
    if (neg)
        x = -x;

    double term = 1.0;
    double sum = 1.0;
    for (int n = 1; n < 30; n++)
    {
        term *= x / n;
        sum += term;
        if (term < 1e-12)
            break;
    }

    return neg ? 1.0 / sum : sum;
}

float sqrtf(float x)
{
    if (x < 0.0f)
        return 0.0f / 0.0f; // NaN
    if (x == 0.0f)
        return 0.0f;

    float guess = x * 0.5f;
    const float eps = 1e-6f;

    for (int i = 0; i < 20; i++)
    {
        if (guess == 0.0f)
            break;
        float new_guess = 0.5f * (guess + x / guess);
        if (abs(new_guess - guess) < eps)
            break;
        guess = new_guess;
    }

    return guess;
}
double sqrtd(double x)
{
    if (x < 0.0)
        return 0.0 / 0.0; // NaN
    if (x == 0.0)
        return 0.0;

    double guess = x * 0.5;
    const double eps = 1e-12;

    for (int i = 0; i < 30; i++)
    {
        if (guess == 0.0)
            break;
        double new_guess = 0.5 * (guess + x / guess);
        if (abs(new_guess - guess) < eps)
            break;
        guess = new_guess;
    }

    return guess;
}

float powf(float base, float exp)
{
    if (base == 0.0f)
        return 0.0f;
    if (exp == 0.0f)
        return 1.0f;

    float ln_base = lnf(base);
    return expf(exp * ln_base);
}
double powd(double base, double exp)
{
    if (base == 0.0)
        return 0.0;
    if (exp == 0.0)
        return 1.0;

    double ln_base = lnd(base);
    return expd(exp * ln_base);
}

int32_t pow_int32(int32_t base, int32_t exp)
{
    int32_t result = 1;
    while (exp > 0)
    {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}
uint32_t pow_uint32(uint32_t base, uint32_t exp)
{
    uint32_t result = 1;
    while (exp > 0)
    {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}
int64_t pow_int64(int64_t base, int64_t exp)
{
    int64_t result = 1;
    while (exp > 0)
    {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}
uint64_t pow_uint64(uint64_t base, uint64_t exp)
{
    uint64_t result = 1;
    while (exp > 0)
    {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}