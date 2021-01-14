#pragma once

#include "Vector3.h"
#include "Triplet.h"
#include "Hittable.h"

Triplet random_vector_unit_half_sphere(Triplet normal_);

Triplet random_vector_sphere(double radius_);

Colour calculate_colour(const Vector3 &ray_, const Hittable &scene_, int depth_);

void gamma_correction(Colour &colour_, double gamma_);

double my_rand();

Vector3 reflect(const Vector3 &ray_, const Triplet &n_);

double dot(const Vector3 &v1, const Triplet &v2);
