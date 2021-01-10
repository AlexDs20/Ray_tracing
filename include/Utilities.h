#pragma once

#include "Vector3.h"
#include "Triplet.h"
#include "Hittable.h"

Triplet random_vector_unit_half_sphere(Triplet normal_);

Colour calculate_colour(const Vector3 &ray_, const Hittable &scene_, int depth_);
