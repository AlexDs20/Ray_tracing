#pragma once
#include "Triplet.h"
#include "Vector3.h"

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
