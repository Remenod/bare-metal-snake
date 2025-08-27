#include <lib/math.h>

float fmodf(float x, float y)
{
    if (y == 0.0f)
        return 0.0f;
    int sign = x < 0 ? -1 : 1;
    x = (x < 0) ? -x : x;
    y = (y < 0) ? -y : y;
    int32_t n = (int32_t)(x / y);
    return sign * (x - n * y);
}
double fmodd(double x, double y)
{
    if (y == 0.0)
        return 0.0;
    int sign = x < 0 ? -1 : 1;
    x = (x < 0) ? -x : x;
    y = (y < 0) ? -y : y;
    int64_t n = (int64_t)(x / y);
    return sign * (x - n * y);
}

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

float sinf(float x)
{
    x = fmod(x, 2.0f * PI);
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
    x = fmod(x, 2.0f * PI);
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

float asinf(float x) { return x + (x * x * x) / 6.0f + (3 * x * x * x * x * x) / 40.0f; }
double asind(double x) { return x + (x * x * x) / 6.0 + (3 * x * x * x * x * x) / 40.0; }

float acosf(float x) { return PI / 2 - asinf(x); }
double acosd(double x) { return PI / 2 - asind(x); }

float atanf(float x) { return x - x * x * x / 3.0f + x * x * x * x * x / 5.0f; }
double atand(double x) { return x - x * x * x / 3.0 + x * x * x * x * x / 5.0; }

float actgf(float x) { return PI / 2 - atanf(x); }
double actgd(double x) { return PI / 2 - atand(x); }

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

float sqrtf(float x)
{
    if (x <= 0.0f)
        return 0.0f;

    float guess = x * 0.5f;
    const float eps = 1e-6f;

    for (int i = 0; i < 20; i++)
    {
        float new_guess = 0.5f * (guess + x / guess);
        if (abs(new_guess - guess) < eps)
            break;
        guess = new_guess;
    }

    return guess;
}
double sqrtd(double x)
{
    if (x <= 0.0)
        return 0.0;

    double guess = x * 0.5;
    const double eps = 1e-12;

    for (int i = 0; i < 30; i++)
    {
        double new_guess = 0.5 * (guess + x / guess);
        if (abs(new_guess - guess) < eps)
            break;
        guess = new_guess;
    }

    return guess;
}

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

    const float ln2 = 0.69314718f;
    result += k * ln2;
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

    const double ln2 = 0.6931471805599453;
    result += k * ln2;
    return result;
}

float log10f(float x)
{
    const float ln10 = 2.30258509f;
    return ln_fast(x) / ln10;
}
double log10d(double x)
{
    const double ln10 = 2.302585092994046;
    return ln_fast(x) / ln10;
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
