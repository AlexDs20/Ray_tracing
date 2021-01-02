#pragma once
#include "Triplet.h"
#include "Vector3.h"

#include "Define.h"

struct Triangle{
  public:
    Triplet A, B, C;
    Vector3 AB, AC;

  public:
    Triangle() : A(), B(), C() {}
    Triangle(Triplet _A, Triplet _B, Triplet _C) : A(_A), B(_B), C(_C), AB(_A,_B), AC(_A,_C) {}

    const Triplet normal() const {
      return AB.cross(AC);
    }
};

struct Sphere{
  public:
    Triplet center;
    double radius;

  public:
    Sphere() : center(), radius(1) {}
    Sphere(Triplet _c, double _r) : center(_c), radius(_r) {}

    const Triplet normal(const Triplet &pos) const {
      // check if p is on surface
      double dist_to_center = (pos-center).norm();
      if (dist_to_center-radius < EPS && dist_to_center-radius > -EPS )
        return (pos-center)/(pos-center).norm();
    }
};
