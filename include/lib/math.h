#pragma once

#include <lib/types.h>

#define PI 3.14159265358979323846

#define ln(val) _Generic(val, float: lnf, double: lnd)(val)
#define exp(val) _Generic(val, float: expf, double: expd)(val)
#define pow(val) _Generic(val, float: powf, double: powd)(val)
#define sin(val) _Generic(val, float: sinf, double: sind)(val)
#define cos(val) _Generic(val, float: cosf, double: cosd)(val)
#define tan(val) _Generic(val, float: tanf, double: tand)(val)
#define ctg(val) _Generic(val, float: ctgf, double: ctgd)(val)
#define asin(val) _Generic(val, float: asinf, double: asind)(val)
#define acos(val) _Generic(val, float: acosf, double: acosd)(val)
#define atan(val) _Generic(val, float: atanf, double: atand)(val)
#define actg(val) _Generic(val, float: actgf, double: actgd)(val)
#define sinh(val) _Generic(val, float: sinhf, double: sinhd)(val)
#define cosh(val) _Generic(val, float: coshf, double: coshd)(val)
#define tanh(val) _Generic(val, float: tanhf, double: tanhd)(val)
#define sqrt(val) _Generic(val, float: sqrtf, double: sqrtd)(val)
#define fmod(x, y) _Generic(x, float: fmodf, double: fmodd)(x, y)
#define log10(val) _Generic(val, float: log10f, double: log10d)(val)

#define abs(val)            \
    _Generic((val),         \
        int8_t: abs_int8,   \
        int16_t: abs_int16, \
        int32_t: abs_int32, \
        float: absf,        \
        double: absd)(val)

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

static inline int8_t abs_int8(int8_t val)
{
    return val < 0 ? -val : val;
}
static inline int16_t abs_int16(int16_t val)
{
    return val < 0 ? -val : val;
}
static inline int32_t abs_int32(int32_t val)
{
    return val < 0 ? -val : val;
}
static inline float absf(float val)
{
    return val < 0 ? -val : val;
}
static inline double absd(double val)
{
    return val < 0 ? -val : val;
}