#pragma once
#include "Triplet.h"
#include "Vector3.h"
#include "Colour.h"


struct Geometry{
  private:
    Colour colour;

  public:
    // Colour
    Geometry();
    Geometry(const Colour &colour_);

    // Methods
    void set_colour(const Colour &c_);
    const Colour get_colour() const;
};

struct Triangle : public Geometry{
  private:
    const Triplet A, B, C;
    const Vector3 AB, AC;

  public:
    // Constructors
    Triangle();
    Triangle(const Triplet &A_, const Triplet &B_, const Triplet &C_, const Colour &colour_);

    // Methods
    Triplet normal() const;
    bool intersect(const Vector3 &ray, double &t) const;
};

struct Sphere : public Geometry{
  private:
    const Triplet center;
    const double radius;

  public:
    // Constructors
    Sphere();
    Sphere(const Triplet &center_, const double &radius_, const Colour &colour_);

    // Methods
    const Triplet normal(const Triplet &pos) const;
    bool intersect(const Vector3 &ray, double &t) const;
    const Colour get_colour(const Triplet &p_) const;
};
