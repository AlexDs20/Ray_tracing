#pragma once
#include "types.h"

namespace Refractive_Index {
    const f32 AIR = 1.000293f;
    const f32 DIAMOND = 2.4f;
    const f32 GLASS = 1.6f;
    const f32 WATER = 1.33333f;
};

enum Mat_type{
    LAMBERTIAN,
    METAL,
    DIELECTRIC
};

struct Material {
    Mat_type type = Mat_type::LAMBERTIAN;
    f32x3 colour = {1.0f, 1.0f, 1.0f};
    union {
        f32 fuzziness;
        f32 refractive_index;
    };
};

// GEOMETRIES
struct Segment {
    f32x3 a;
    f32x3 b;
};

struct Triangle {
    f32x3 a;
    f32x3 b;
    f32x3 c;
};

struct AABB {
    f32x3 low;
    f32x3 high;
};


struct Ray {
    f32x3 O;
    f32x3 dir;
    f32x3 inv_dir;
    // f32 refrective_index;
};

struct Sphere {
    f32x3 O;
    f32 r;
    u32 material_index;
};

struct World {
    u32 sphere_count;
    Sphere* spheres;

    u32 material_count;
    Material* materials;
};
