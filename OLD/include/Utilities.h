#pragma once

#include <vector>
#include "Vector3.h"
#include "Triplet.h"
#include "Hittable.h"

void rand_in_disc(double r, double *coord);

Triplet random_vector_half_unit_sphere(const Triplet &normal_);

Triplet random_vector_sphere(double radius_);           // Uniformly distributed in r and on each shell surfaces

Triplet random_vector_sphere_uniform(double radius_);   // uniformly distributed in the sphere

Colour calculate_colour(const Vector3 &ray_, const Hittable &scene_, int depth_);

void gamma_correction(Colour &colour_, double gamma_);

double my_rand();
double my_rand(double low, double up);

double dot(const Vector3 &v1, const Triplet &v2);

Vector3 reflect(const Vector3 &ray_, const Triplet &n_);

Vector3 refract(const Vector3 &ray_, const Triplet &n_, double n_over_n_in);
