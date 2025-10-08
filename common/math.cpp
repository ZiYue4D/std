#include "std.hpp"

#include <math.h>
#include <stdlib.h>

_STDLIB_BEGIN

int _STDLIB(Int)(ZStr str) {
    int result = 0;
    int sign = 1;
    const char* ptr = str->c_str();
    while (*ptr == ' ') ptr++;

    if (*ptr == '-') {
        sign = -1;
        ptr++;
    } else if (*ptr == '+') {
        ptr++;
    }

    while (*ptr >= '0' && *ptr <= '9') {
        result = result * 10 + (*ptr - '0');
        ptr++;
    }

    return result * sign;
}

float _STDLIB(Float)(ZStr str) {
    float result = 0.0f;
    int sign = 1;
    const char* ptr = str->c_str();
    while (*ptr == ' ') ptr++;

    if (*ptr == '-') {
        sign = -1;
        ptr++;
    } else if (*ptr == '+') {
        ptr++;
    }

    while (*ptr >= '0' && *ptr <= '9') {
        result = result * 10.0f + (*ptr - '0');
        ptr++;
    }

    if (*ptr == '.') {
        ptr++;
        float decimalPlace = 0.1f;
        while (*ptr >= '0' && *ptr <= '9') {
            result += (*ptr - '0') * decimalPlace;
            decimalPlace *= 0.1f;
            ptr++;
        }
    }

    return result * sign;
}

_ALWAYS_INLINE
float _STDLIB(Floor)(float x) {
    return floorf(x);
}

_ALWAYS_INLINE
float _STDLIB(Ceil)(float x) {
    return ceilf(x);
}

_ALWAYS_INLINE
float _STDLIB(Sin)(float x) {
    return sinf(x);
}

_ALWAYS_INLINE
float _STDLIB(Cos)(float x) {
    return cosf(x);
}

_ALWAYS_INLINE
float _STDLIB(Tan)(float x) {
    return tanf(x);
}

_ALWAYS_INLINE
float _STDLIB(Sqr)(float x) {
    return sqrtf(x);
}

_ALWAYS_INLINE
float _STDLIB(Round)(float x) {
    return roundf(x);
}

_STDLIB_END