#pragma once
#include "types.h"

f32 random_in_range(f32 low, f32 high) {            // inclusive min and max
    return low + (f32)((f64)rand() / (f64)RAND_MAX) * (high - low);
}

f32x3 random_vector_on_unit_sphere() {
    while(true) {
        f32 x = random_in_range(-1.0f, 1.0f);
        f32 y = random_in_range(-1.0f, 1.0f);
        f32 z = random_in_range(-1.0f, 1.0f);
        if (x*x+y*y+z*z <= 1.0f) return normalize({x, y, z});
    }
}

f32x3 random_vector_in_unit_sphere() {
    while(true) {
        f32 x = random_in_range(-1.0f, 1.0f);
        f32 y = random_in_range(-1.0f, 1.0f);
        f32 z = random_in_range(-1.0f, 1.0f);
        if (x*x+y*y+z*z <= 1.0f) return {x, y, z};
    }
}
