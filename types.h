#pragma once
#include <cstdint>
#include <math.h>

typedef char unsigned u8;
typedef short unsigned u16;
typedef int unsigned u32;

typedef char s8;
typedef short s16;
typedef int s32;

typedef float f32;
typedef double f64;

struct f32x3 {
    union {
        f32 data[3];
        struct {
            f32 x, y, z;
        };
        struct {
            f32 r, g, b;
        };
    };
};

struct f32x4 {
    union {
        f32 data[4];
        struct {
            f32 x, y, z, w;
        };
        struct {
            f32 r, g, b, a;
        };
    };
};

// typedef uint32_t u32;
// typedef uint8_t u8;
//
// struct u32x3 {
//     union {
//         u32 data[3];
//         struct {
//             u32 x, y, z;
//         };
//     };
// };


f32x3 cross(const f32x3 a, const f32x3 b) {
    f32x3 out;
    out.x = a.y*b.z - a.z*b.y;
    out.y = a.z*b.x - a.x*b.z;
    out.z = a.x*b.y - a.y*b.x;
    return out;
}
f32 dot(const f32x3 a, const f32x3 b) {
    return a.x*b.x + a.y*b.y + a.z*b.z;
}

f32x3 operator+(float a, f32x3 v) {
    return { a+v.x, a+v.y, a+v.z };
}
f32x3 operator+(f32x3 v, float a) {
    return { a+v.x, a+v.y, a+v.z };
}
f32x3 operator-(f32x3 v) {
    return { -v.x, -v.y, -v.z };
}
f32x3 operator-(float a, f32x3 v) {
    return { a-v.x, a-v.y, a-v.z };
}
f32x3 operator-(f32x3 v, float a) {
    return { v.x-a, v.y-a, v.z-a };
}
f32x3 operator*(float a, f32x3 v) {
    return { a*v.x, a*v.y, a*v.z };
}
f32x3 operator*(f32x3 v, float a) {
    return { a*v.x, a*v.y, a*v.z };
}
f32x3 operator/(f32x3 v, float a) {
    return { v.x/a, v.y/a, v.z/a };
}
f32x3 operator-(f32x3 a, f32x3 v) {
    return { a.x-v.x, a.y-v.y, a.z-v.z };
}
f32x3 operator+(f32x3 a, f32x3 v) {
    return { a.x+v.x, a.y+v.y, a.z+v.z };
}
f32x3 operator*(f32x3 a, f32x3 v) {
    return { a.x*v.x, a.y*v.y, a.z*v.z };
}
f32 length(f32x3 a) {
    return sqrt(a.x*a.x + a.y*a.y + a.z*a.z);
}
f32 length2(f32x3 a) {
    return a.x*a.x + a.y*a.y + a.z*a.z;
}
f32x3 normalize(f32x3 a) {
    f32 norm = length(a);
    return { a.x/norm, a.y/norm, a.z/norm };
}
f32x3& operator*=(f32x3& left, const f32x3 right) {
    left.x *= right.x;
    left.y *= right.y;
    left.z *= right.z;
    return left;
}
f32x3& operator+=(f32x3& left, const f32x3 right) {
    left.x += right.x;
    left.y += right.y;
    left.z += right.z;
    return left;
}
f32x3 sqrt(f32x3 a) {
    return { sqrt(a.x), sqrt(a.y), sqrt(a.z) };
}
f32x3 pow(f32x3 a, f32 power) {
    return {
        pow(a.x, power),
        pow(a.y, power),
        pow(a.z, power),
    };
}
