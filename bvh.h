#pragma once
#include <stdio.h>
#include <float.h>
#include "types.h"
#include "object.h"
#include "intersect.h"


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

    aabb.low = HadamardMin(tri.a, HadamardMin(tri.b, tri.c));
    aabb.high = HadamardMax(tri.a, HadamardMax(tri.b, tri.c));

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

    aabb.low = HadamardMin(s.a, s.b);
    aabb.high = HadamardMax(s.a, s.b);

    return aabb;
}


struct BVHNode {
    AABB bbox = {.low={FLT_MAX, FLT_MAX, FLT_MAX}, .high={-FLT_MAX, -FLT_MAX, -FLT_MAX}};
    union {
        u32 leftIndex;
        u32 startObjects;
    };
    u32 nObjects;

};


AABB create_aabb_from_objects(Sphere* objects, u32 N) {
    AABB bbox;
    bbox.low = {FLT_MAX, FLT_MAX, FLT_MAX};
    bbox.high = {-FLT_MAX, -FLT_MAX, -FLT_MAX};
    for (u32 i=0; i<N; i++) {
        bbox.low = HadamardMin(bbox.low, objects[i].O - objects[i].r);
        bbox.high = HadamardMax(bbox.high, objects[i].O + objects[i].r);
    }

    return bbox;
}


void split_node(BVHNode* tree, BVHNode* node, Sphere* objects, u32& nodes_used) {
    if (node->nObjects<=1) return;

    // Get axis to split bbox
    AABB& bbox = node->bbox;
    f32x3 size = bbox.high - bbox.low;
    u8 axis = size.x < size.y ? 1 : 0;
    axis = size.data[axis] < size.z ? 2 : axis;
    f32 split = bbox.low.data[axis] + size.data[axis] * 0.5f;

    // Sort the objects between either sides of bbox
    u32 i = node->startObjects;
    u32 j = i + node->nObjects - 1;

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

    if (i==node->startObjects || i==node->startObjects+node->nObjects) {
        return;
    }

    // Save temporarily where the objects starts
    // This is because startObjects and leftIndex use the same "bytes/variable"
    u32 startObjects = node->startObjects;

    // Create the nodes
    // Assign the leaves and reset the associated objects then
    node->leftIndex = nodes_used++;
    nodes_used++;                       // for right index

    BVHNode* leftNode = &tree[node->leftIndex];
    BVHNode* rightNode = &tree[node->leftIndex+1];

    leftNode->startObjects = startObjects;
    leftNode->nObjects = i - startObjects;

    rightNode->startObjects = i;
    rightNode->nObjects = node->nObjects - leftNode->nObjects;

    leftNode->bbox = create_aabb_from_objects(&objects[leftNode->startObjects], leftNode->nObjects);
    rightNode->bbox = create_aabb_from_objects(&objects[rightNode->startObjects], rightNode->nObjects);

    split_node(tree, leftNode, objects, nodes_used);
    split_node(tree, rightNode, objects, nodes_used);

    node->nObjects = 0;
}


void create_bvh_hierarchy(BVHNode* tree, Sphere* objects, u32 N) {
    u32 root_node = 0;
    BVHNode* node = &tree[root_node];
    u32 nodes_used = 1;

    // Get BBOX of objects
    node->bbox = create_aabb_from_objects(objects, N);

    node->leftIndex = 0;

    node->startObjects = 0;
    node->nObjects = N;

    split_node(tree, node, objects, nodes_used);
}

void traverse_bvh_hierarchy(const Ray& ray, BVHNode* tree, u32 nodeIdx, const Sphere* objects, f32* t, s32* idx_obj) {
    const BVHNode& node = tree[nodeIdx];

    f32 t_entry = ray_aabb_intersect(ray, node.bbox);
    // TODO(alex): better check, we could be IN the bbox
    if (t_entry <= 0.0f) {
        return;
    }

    if (node.nObjects != 0) {
        for (u32 i=0; i<node.nObjects; i++) {
            f32 tmp = ray_sphere_intersect(ray, objects[node.startObjects+i]);
            if ((tmp>0.0f) && (tmp < *t)){
                *t = tmp;
                *idx_obj = node.startObjects+i;
            }
        }
        return;
    }

    traverse_bvh_hierarchy(ray, tree, node.leftIndex,   objects, t, idx_obj);
    traverse_bvh_hierarchy(ray, tree, node.leftIndex+1, objects, t, idx_obj);
}
