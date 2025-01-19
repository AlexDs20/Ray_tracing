#pragma once
#include <float.h>
#include "types.h"
#include "object.h"
#include "stdio.h"


void create_segments_from_aabb(Segment* segments, const AABB& aabb) {
    f32x3 size = aabb.high - aabb.low;
    segments[0].a = aabb.low;
    segments[0].b = aabb.low + f32x3{size.x, 0.0f, 0.0f};

    segments[1].a = aabb.low + f32x3{size.x, 0.0f, 0.0f};
    segments[1].b = aabb.low + f32x3{size.x, size.y, 0.0f};

    segments[2].a = aabb.low + f32x3{size.x, size.y, 0.0f};
    segments[2].b = aabb.low + f32x3{0.0f, size.y, 0.0f};

    segments[3].a = aabb.low + f32x3{0.0f, size.y, 0.0f};
    segments[3].b = aabb.low + f32x3{0.0f, 0.0f, 0.0f};

    segments[4].a = aabb.low + f32x3{0.0f, 0.0f, size.z};
    segments[4].b = aabb.low + f32x3{size.x, 0.0f, size.z};

    segments[5].a = aabb.low + f32x3{size.x, 0.0f, size.z};
    segments[5].b = aabb.low + f32x3{size.x, size.y, size.z};

    segments[6].a = aabb.low + f32x3{size.x, size.y, size.z};
    segments[6].b = aabb.low + f32x3{0.0f, size.y, size.z};

    segments[7].a = aabb.low + f32x3{0.0f, size.y, size.z};
    segments[7].b = aabb.low + f32x3{0.0f, 0.0f, size.z};


    segments[8].a = aabb.low;
    segments[8].b = aabb.low + f32x3{0.0f, 0.0f, size.z};

    segments[9].a = aabb.low + f32x3{size.x, 0.0f, 0.0f};
    segments[9].b = aabb.low + f32x3{size.x, 0.0f, size.z};

    segments[10].a = aabb.low + f32x3{0.0f, size.y, 0.0f};
    segments[10].b = aabb.low + f32x3{0.0f, size.y, size.z};

    segments[11].a = aabb.low + f32x3{size.x, size.y, 0.0f};
    segments[11].b = aabb.low + f32x3{size.x, size.y, size.z};
}


AABB aabb_triangle(const Triangle& tri) {
    AABB aabb;

    aabb.low = f32x3min(tri.a, f32x3min(tri.b, tri.c));
    aabb.high = f32x3max(tri.a, f32x3max(tri.b, tri.c));

    return aabb;
}


AABB aabb_sphere(const Sphere& s) {
    AABB aabb;

    aabb.low = s.O - s.r;
    aabb.high = s.O + s.r;

    return aabb;
}


AABB aabb_segment(const Segment& s) {
    AABB aabb;

    aabb.low = f32x3min(s.a, s.b);
    aabb.high = f32x3max(s.a, s.b);

    return aabb;
}


struct BVHNode {
    AABB bbox = {.low={FLT_MAX, FLT_MAX, FLT_MAX}, .high={-FLT_MAX, -FLT_MAX, -FLT_MAX}};
    BVHNode* left;
    BVHNode* right;
    bool is_leaf;
    Sphere* objects;
    u32 N;
};

void print(f32x3 v) {
    printf("(%.5f,%.5f,%.5f)\n", v.x,  v.y, v.z );
}

void print(f32 v) {
    printf("%.5f\n", v);
}

AABB create_aabb_from_objects(Sphere* objects, u32 N) {
    AABB bbox;
    bbox.low = {FLT_MAX, FLT_MAX, FLT_MAX};
    bbox.high = {-FLT_MAX, -FLT_MAX, -FLT_MAX};
    for (u32 i=0; i<N; i++) {
        bbox.low = f32x3min(bbox.low, objects[i].O - objects[i].r);
        bbox.high = f32x3max(bbox.high, objects[i].O + objects[i].r);
    }

    return bbox;
}


void split_node(BVHNode* tree, BVHNode* node, Sphere* objects, u32 N, u32& nodes_used) {
    if (N<=1) return;
    // Get axis to split bbox
    AABB& bbox = node->bbox;
    f32x3 size = bbox.high - bbox.low;
    u8 axis = size.x < size.y ? 1 : 0;
    axis = size.data[axis] < size.z ? 2 : axis;
    f32 split = bbox.low.data[axis] + size.data[axis] * 0.5f;

    // Sort the objects between either sides of bbox
    u32 j = N-1;
    u32 i = 0;
    Sphere tmp;
    while (i<=j) {
        if (objects[i].O.data[axis] < split) {
            i++;
        } else {
            // swap elements
            tmp = objects[i];
            objects[i] = objects[j];
            objects[j] = tmp;
            j--;
        }
    }


    node->left = &tree[nodes_used++];
    node->left->bbox = create_aabb_from_objects(&objects[0], i);
    node->left->objects = &objects[0];
    node->left->N = i;

    node->right = &tree[nodes_used++];
    node->right->bbox = create_aabb_from_objects(&objects[i], N-i);
    node->right->objects = &objects[i];
    node->right->N = N-i;

    split_node(tree, node->left, node->left->objects, node->left->N, nodes_used);
    split_node(tree, node->right, node->right->objects, node->right->N, nodes_used);
}


void create_bvh_hierarchy(BVHNode* tree, Sphere* objects, u32 N) {
    BVHNode* node = &tree[0];
    u32 nodes_used = 1;

    // Get BBOX of objects
    node->bbox = create_aabb_from_objects(objects, N);
    AABB& bbox = node->bbox;

    node->objects = objects;
    node->N = N;

    split_node(tree, node, node->objects, node->N, nodes_used);
}
