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
    AABB bbox; // = {.low={FLT_MAX, FLT_MAX, FLT_MAX}, .high={-FLT_MAX, -FLT_MAX, -FLT_MAX}};
    u32 startIndex;
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


inline f32 aabb_area(AABB& bbox) {
    const f32x3 extent = bbox.high - bbox.low;
    return extent.x*extent.y + extent.y*extent.z + extent.x*extent.z;
}


inline f32 compute_node_cost(BVHNode* node) {
    return aabb_area(node->bbox) * node->nObjects;
}


f32 evaluate_split_SAH(BVHNode* node, Sphere* objects, u8 axis, f32 pos) {
    AABB left  = {.low={FLT_MAX, FLT_MAX, FLT_MAX}, .high={-FLT_MAX, -FLT_MAX, -FLT_MAX}};
    AABB right = {.low={FLT_MAX, FLT_MAX, FLT_MAX}, .high={-FLT_MAX, -FLT_MAX, -FLT_MAX}};
    s32 left_count = 0, right_count = 0;
    for (u32 i=0; i<node->nObjects; i++) {
        Sphere object = objects[node->startIndex + i];
        if (object.O.data[axis]<pos) {
            left.low = HadamardMin(left.low, object.O-object.r);
            left.high = HadamardMax(left.high, object.O+object.r);
            left_count++;
        } else {
            right.low = HadamardMin(right.low, object.O-object.r);
            right.high = HadamardMax(right.high, object.O+object.r);
            right_count++;
        }
    }
    return left_count * aabb_area(left) + right_count * aabb_area(right);
}


void split_node(BVHNode* tree, Sphere* objects, BVHNode* node, u32& nodes_used) {
    if (node->nObjects<=1) return;

    // Get axis to split bbox
    AABB& bbox = node->bbox;
    f32x3 size = bbox.high - bbox.low;
    u8 axis = size.x < size.y ? 1 : 0;
    axis = size.data[axis] < size.z ? 2 : axis;
    f32 split = bbox.low.data[axis] + size.data[axis] * 0.5f;

    // Sort the objects between either sides of bbox
    u32 i = node->startIndex;
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

    if (i==node->startIndex || i==node->startIndex+node->nObjects) {
        return;
    }

    // Save temporarily where the objects starts
    // This is because startIndex and startIndex use the same "bytes/variable"
    u32 startIndex = node->startIndex;

    // Create the nodes
    // Assign the leaves and reset the associated objects then
    node->startIndex = nodes_used++;
    nodes_used++;                       // for right index

    BVHNode* leftNode = &tree[node->startIndex];
    BVHNode* rightNode = &tree[node->startIndex+1];

    leftNode->startIndex = startIndex;
    leftNode->nObjects = i - startIndex;

    rightNode->startIndex = i;
    rightNode->nObjects = node->nObjects - leftNode->nObjects;

    leftNode->bbox = create_aabb_from_objects(&objects[leftNode->startIndex], leftNode->nObjects);
    rightNode->bbox = create_aabb_from_objects(&objects[rightNode->startIndex], rightNode->nObjects);

    split_node(tree, objects, leftNode, nodes_used);
    split_node(tree, objects, rightNode, nodes_used);

    node->nObjects = 0;
}


void split_node_alt(BVHNode* tree, Sphere* objects, BVHNode* node, u32& nodes_used) {
    // Get axis to split bbox
    AABB& bbox = node->bbox;
    f32x3 extent = bbox.high - bbox.low;
    u8 axis = extent.x < extent.y ? 1 : 0;
    axis = extent.data[axis] < extent.z ? 2 : axis;

    // Test the splits
    const u32 n_steps = 8;
    const f32 step = extent.data[axis] / (n_steps+1);

    f32 best_cost = FLOAT_MAX;
    f32 best_pos;
    f32 split_pos = node->bbox.low.data[axis];
    for (u32 i=1; i<=n_steps; i++) {
        split_pos += step;
        f32 cost = evaluate_split_SAH(node, objects, axis, split_pos);
        if (cost<best_cost) {
            best_cost = cost;
            best_pos = split_pos;
        }
    }

    // check if it's work splitting
    f32 this_cost = compute_node_cost(node);
    if (best_cost>=this_cost) {
        return;
    }
    const f32 split = best_pos;

    // Sort the objects between either sides of bbox
    u32 i = node->startIndex;
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

    if (i==node->startIndex || i==node->startIndex+node->nObjects) {
        return;
    }

    // Save temporarily where the objects starts
    // This is because startIndex and startIndex use the same "bytes/variable"
    u32 startIndex = node->startIndex;

    // Create the nodes
    // Assign the leaves and reset the associated objects then
    node->startIndex = nodes_used++;
    nodes_used++;                       // for right index

    BVHNode* leftNode = &tree[node->startIndex];
    BVHNode* rightNode = &tree[node->startIndex+1];

    leftNode->startIndex = startIndex;
    leftNode->nObjects = i - startIndex;

    rightNode->startIndex = i;
    rightNode->nObjects = node->nObjects - leftNode->nObjects;

    leftNode->bbox = create_aabb_from_objects(&objects[leftNode->startIndex], leftNode->nObjects);
    rightNode->bbox = create_aabb_from_objects(&objects[rightNode->startIndex], rightNode->nObjects);

    split_node_alt(tree, objects, leftNode, nodes_used);
    split_node_alt(tree, objects, rightNode, nodes_used);

    node->nObjects = 0;
}


void create_bvh_hierarchy(BVHNode* tree, Sphere* objects, u32 N) {
    u32 root_node = 0;
    BVHNode* node = &tree[root_node];
    u32 nodes_used = 1;

    // Get BBOX of objects
    node->bbox = create_aabb_from_objects(objects, N);

    node->startIndex = 0;

    node->startIndex = 0;
    node->nObjects = N;

    // split_node(tree, objects, node, nodes_used);
    split_node_alt(tree, objects, node, nodes_used);
}

void traverse_bvh_hierarchy(const Ray& ray, BVHNode* tree, u32 nodeIdx, const Sphere* objects, f32* t, s32* idx_obj, u32& depth) {
    const BVHNode& node = tree[nodeIdx];

    f32 t_entry = ray_aabb_intersect(ray, node.bbox);
    // TODO(alex): better check, we could be IN the bbox
    if (t_entry == FLOAT_MAX) {
        return;
    }

    if (node.nObjects != 0) {
        depth++;
        for (u32 i=0; i<node.nObjects; i++) {
            f32 tmp = ray_sphere_intersect(ray, objects[node.startIndex+i]);
            if ((tmp>0.0f) && (tmp < *t)){
                *t = tmp;
                *idx_obj = node.startIndex+i;
            }
        }
        return;
    }
    depth++;

    traverse_bvh_hierarchy(ray, tree, node.startIndex,   objects, t, idx_obj, depth);
    traverse_bvh_hierarchy(ray, tree, node.startIndex+1, objects, t, idx_obj, depth);
}


void print(const BVHNode* node) {
    printf(
            "min: (%.5f,%.5f,%.5f), max: (%.5f,%.5f,%.5f), startIdx: %d, nObjects: %d\n",
            node->bbox.low.x, node->bbox.low.y, node->bbox.low.z,
            node->bbox.high.x, node->bbox.high.y, node->bbox.high.z,
            node->startIndex,
            node->nObjects
    );
}

#include <stdint.h>

template <class T>
void swap(T& a, T& b) {
    // a = (T)((uintptr_t)a + (uintptr_t)b);
    // b = (T)((uintptr_t)a - (uintptr_t)b);
    // a = (T)((uintptr_t)a - (uintptr_t)b);

    // a = (T)((uintptr_t)a ^ (uintptr_t)b);
    // b = (T)((uintptr_t)a ^ (uintptr_t)b);
    // a = (T)((uintptr_t)a ^ (uintptr_t)b);

    T tmp = a;
    a = b;
    b = tmp;
}

u32 traverse_bvh_hierarchy_non_rec(const Ray& ray, BVHNode* tree, u32 nodeIdx, const Sphere* objects, f32* t, s32* idx_obj) {
    BVHNode* node = &tree[nodeIdx];

    // Works up to 4_000_000_000 objects (2^32 as long as I use binary trees)
    BVHNode* stack[32];
    s32 stack_idx = -1;

    u32 depth = 1;

    while(1) {
        if (node->nObjects != 0) {
            // Go through each triangles
            f32 tmp = FLOAT_MAX;
            for (u32 i=0; i<node->nObjects; i++) {
                tmp = ray_sphere_intersect(ray, objects[node->startIndex + i]);
                if ((tmp>0.0f) && (tmp<*t)) {
                    *t = tmp;
                    *idx_obj = node->startIndex+i;
                }
            }
            if (stack_idx < 0) {
                break;
            } else {
                node = stack[stack_idx--];
                continue;
            }
        } else {
            BVHNode* node1 = &tree[node->startIndex];
            BVHNode* node2 = &tree[node->startIndex+1];

            f32 t1 = ray_aabb_intersect(ray, node1->bbox);
            f32 t2 = ray_aabb_intersect(ray, node2->bbox);

            if (t1 > t2) {
                swap(t1, t2);
                swap(node1, node2);
            }
            if (t1 == FLOAT_MAX) {
                if (stack_idx >= 0) {
                    depth++;
                    node = stack[stack_idx--];
                } else {
                    break;
                }
            } else  {
                depth++;
                node = node1;
                if (t2 != FLOAT_MAX) {
                    stack[++stack_idx] = node2;
                }
            }
        }
    }
    return depth;
}
