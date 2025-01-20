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
    u32 leftIndex;
    u32 rightIndex;
    u32 startObjects;
    u32 nObjects;
};

void print(f32x3 v) {
    printf("(%.5f,%.5f,%.5f)\n", v.x,  v.y, v.z );
}

void print(f32 v) {
    printf("%.5f\n", v);
}

void print(){
    printf("======================\n");
};

void print(BVHNode* node, u32 nodes_used=-1) {
    printf(
        "node_used: %d\nleftIdx: %d\nrightIdx: %d\nstartObj: %d\nnObjects: %d\n",
        nodes_used,
        node->leftIndex,
        node->rightIndex,
        node->startObjects,
        node->nObjects
    );
}


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

    // Assign the leaves and reset the associated objects then
    node->leftIndex = nodes_used++;
    node->rightIndex = nodes_used++;

    // Create the nodes
    BVHNode* leftNode = &tree[node->leftIndex];
    BVHNode* rightNode = &tree[node->rightIndex];

    leftNode->startObjects = node->startObjects;
    leftNode->nObjects = i - node->startObjects;

    rightNode->startObjects = i;
    rightNode->nObjects = node->nObjects - leftNode->nObjects;

    leftNode->bbox = create_aabb_from_objects(&objects[leftNode->startObjects], leftNode->nObjects);
    rightNode->bbox = create_aabb_from_objects(&objects[rightNode->startObjects], rightNode->nObjects);

    split_node(tree, leftNode, objects, nodes_used);
    split_node(tree, rightNode, objects, nodes_used);

    node->startObjects = 0;
    node->nObjects = 0;
}


void create_bvh_hierarchy(BVHNode* tree, Sphere* objects, u32 N) {
    u32 root_node = 0;
    BVHNode* node = &tree[root_node];
    u32 nodes_used = 1;

    // Get BBOX of objects
    node->bbox = create_aabb_from_objects(objects, N);

    node->leftIndex = 0;
    node->rightIndex = 0;

    node->startObjects = 0;
    node->nObjects = N;

    split_node(tree, node, objects, nodes_used);
}
