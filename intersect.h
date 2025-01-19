#pragma once
#include "types.h"
#include "object.h"

f32 ray_sphere_intersect(Ray& ray, Sphere& sphere) {
    f32x3 C = sphere.O;
    f32x3 O = ray.O;
    f32x3 OC = C-O;
    f32 half_b = dot(ray.dir, OC);
    f32 a = 1.0f;       // length2(ray.dir);
    f32 c = length2(OC) - sphere.r*sphere.r;

    f32 delta = half_b*half_b - a * c;

    if (delta < 0) {
        return 0.0f;
    }

    f32 t = (half_b - sqrt(delta))/a;

    if (abs(t) > EPS) {
        return t;
    }

    t = (half_b + sqrt(delta))/a;
    return t;
}

f32 ray_segment_intersect(Ray& ray, Segment& segment) {
    // referens: https://mathworld.wolfram.com/Line-LineIntersection.html
    f32x3 a = segment.b - segment.a;
    const f32x3& b = ray.dir;
    f32x3 c = ray.O - segment.a;

    f32x3 axb = cross(a, b);

    // Check that not coplanar
    if ( abs(dot(c, axb)) > 0.005f) {
        return 0.0f;
    }

    f32 s = dot(cross(c, b), axb) / length2(axb);
    if ((s >= 0) && (s <= 1)) {
        f32 t = dot(cross(c, a), axb) / length2(axb);
        return t;
    }
    return 0.0f;
}

f32 ray_triangles_intersect(Ray& ray, Triangle& triangle) {
    const f32x3 edge1 = triangle.b - triangle.a;
    const f32x3 edge2 = triangle.c - triangle.a;
    const f32x3 h = cross( ray.dir, edge2 );
    const f32 a = dot( edge1, h );

    if ( abs(a) < EPS ) {
        return 0.0f; // ray parallel to triangle
    }

    const f32 f = 1.0f / a;
    const f32x3 s = ray.O - triangle.a;
    const f32 u = f * dot( s, h );

    if (u < 0 || u > 1) {
        return 0.0f;
    }

    const f32x3 q = cross( s, edge1 );
    const f32 v = f * dot( ray.dir, q );
    if (v < 0 || u + v > 1) {
        return 0.0f;
    }

    const float t = f * dot( edge2, q );
    if (t > EPS) {
        return t;
    }

    return 0.0f;
}
