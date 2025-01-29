#pragma once
#include <math.h>
#include <stdio.h>
#include "types.h"
#include "object.h"

// #define __AVX
#ifdef __AVX
#include <immintrin.h>
#endif

f32 ray_sphere_intersect(const Ray& ray, const Sphere& sphere) {
    f32x3 C = sphere.O;
    f32x3 O = ray.O;
    f32x3 OC = C-O;
    f32 half_b = dot(ray.dir, OC);
    f32 a = 1.0f;       // length2(ray.dir);
    f32 c = length2(OC) - sphere.r*sphere.r;

    f32 delta = half_b*half_b - a * c;

    if (delta < 0) {
        return FLOAT_MAX;
    }

    f32 t = (half_b - sqrt(delta))/a;

    if (abs(t) > EPS) {
        return t;
    }

    t = (half_b + sqrt(delta))/a;
    return t;
}

f32 ray_segment_intersect(const Ray& ray, const Segment& segment) {
    // referens: https://mathworld.wolfram.com/Line-LineIntersection.html
    f32x3 a = segment.b - segment.a;
    const f32x3& b = ray.dir;
    f32x3 c = ray.O - segment.a;

    f32x3 axb = cross(a, b);

    // Check that not coplanar
    if ( abs(dot(c, axb)) > 0.005f) {
        return FLOAT_MAX;
    }

    f32 s = dot(cross(c, b), axb) / length2(axb);
    if ((s >= 0) && (s <= 1)) {
        f32 t = dot(cross(c, a), axb) / length2(axb);
        return t;
    }
    return FLOAT_MAX;
}

f32 ray_triangles_intersect(const Ray& ray, const Triangle& triangle) {
    const f32x3 edge1 = triangle.b - triangle.a;
    const f32x3 edge2 = triangle.c - triangle.a;
    const f32x3 h = cross( ray.dir, edge2 );
    const f32 a = dot( edge1, h );

    if ( abs(a) < EPS ) {
        return FLOAT_MAX; // ray parallel to triangle
    }

    const f32 f = 1.0f / a;
    const f32x3 s = ray.O - triangle.a;
    const f32 u = f * dot( s, h );

    if (u < 0 || u > 1) {
        return FLOAT_MAX;
    }

    const f32x3 q = cross( s, edge1 );
    const f32 v = f * dot( ray.dir, q );
    if (v < 0 || u + v > 1) {
        return FLOAT_MAX;
    }

    const float t = f * dot( edge2, q );
    if (t > EPS) {
        return t;
    }

    return FLOAT_MAX;
}

f32 ray_aabb_intersect(const Ray& ray, const AABB& aabb) {
#ifndef __AVX
    // f32x3 t1 = HadamardDivision(aabb.low - ray.O, ray.dir);
    // f32x3 t2 = HadamardDivision(aabb.high - ray.O, ray.dir);
    f32x3 t1 = (aabb.low - ray.O) * ray.inv_dir;
    f32x3 t2 = (aabb.high - ray.O) * ray.inv_dir;

    f32x3 t_in = HadamardMin(t1, t2);
    f32x3 t_out = HadamardMax(t1, t2);

    f32 t_exit = f32min(t_out.z, f32min(t_out.x, t_out.y));
    f32 t_entry = f32max(t_in.z, f32max(t_in.x, t_in.y));

    if (t_exit > 0 && t_entry < t_exit) {
        return t_entry;
    } else {
        return FLOAT_MAX;
    }
#else
    // I get no improvements if -O2 => probably this is what the compiler does if using non vectorized code.
    const __m128i mask = _mm_set_epi32(0, -1, -1, -1);
    __m128 low = _mm_maskload_ps(&aabb.low.x, mask);
    __m128 high = _mm_maskload_ps(&aabb.high.x, mask);
    __m128 inv_dir = _mm_maskload_ps(&ray.inv_dir.x, mask);
    __m128 ray_o = _mm_maskload_ps(&ray.O.x, mask);

    __m128 t1 = _mm_mul_ps(_mm_sub_ps(low, ray_o), inv_dir);
    __m128 t2 = _mm_mul_ps(_mm_sub_ps(high, ray_o), inv_dir);

    __m128 t_in = _mm_min_ps(t1, t2);
    __m128 t_out = _mm_max_ps(t1, t2);

    float t_in_f[4];
    float t_out_f[4];
    _mm_store_ps(&t_in_f[0], t_in);
    _mm_store_ps(&t_out_f[0], t_out);

    f32 t_exit = f32min(t_out_f[2], f32min(t_out_f[0], t_out_f[1]));
    f32 t_entry = f32max(t_in_f[2], f32max(t_in_f[0], t_in_f[1]));

    if (t_exit > 0 && t_entry < t_exit) {
        return t_entry;
    } else {
        return FLOAT_MAX;
    }
#endif
}
