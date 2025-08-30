#pragma once

#include <lib/math.h>

#define fmod(x, y) _Generic(x, float: fmodf, double: fmodd)(x, y)

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

#define ln(val) _Generic(val, float: lnf, double: lnd)(val)
#define log10(val) _Generic(val, float: log10f, double: log10d)(val)
#define logb(val, base) _Generic(val, float: logbf, double: logbd)(val, base)

#define ceil(val) _Generic(val, float: ceilf, double: ceild)(val)
#define floor(val) _Generic(val, float: floorf, double: floord)(val)
#define round(val) _Generic(val, float: roundf, double: roundd)(val)

#define exp(val) _Generic(val, float: expf, double: expd)(val)
#define sqrt(val) _Generic(val, float: sqrtf, double: sqrtd)(val)
#define pow(base, exp)        \
    _Generic(base,            \
        float: powf,          \
        double: powd,         \
        int32_t: pow_int32,   \
        uint32_t: pow_uint32, \
        int64_t: pow_int64,   \
        uint64_t: pow_uint64)(base, exp)

#define abs(val)            \
    _Generic(val,           \
        int8_t: abs_int8,   \
        int16_t: abs_int16, \
        int32_t: abs_int32, \
        int64_t: abs_int64, \
        float: absf,        \
        double: absd)(val)

#define min(a, b)     \
    _Generic(a,       \
        float: minf,  \
        double: mind, \
        default: min_int)(a, b)

#define max(a, b)     \
    _Generic(a,       \
        float: maxf,  \
        double: maxd, \
        default: max_int)(a, b)
