#pragma once
#include "Triplet.h"
#include "Vector3.h"
#include "Colour.h"

#include "Define.h"

struct Geometry{
  private:
    Colour colour;

  public:
    Geometry() : colour(Colour(0,0,255)) {}
    Geometry(const Colour &c_) : colour(c_) {}

    void set_colour(const Colour &c_){
      colour = c_;
    }

    const Colour get_colour() const{
      return colour;
    }
};

struct Triangle : public Geometry{
  public:
    Triplet A, B, C;
    Vector3 AB, AC;

  public:
    Triangle() : Geometry(), A(), B(), C() {}
    Triangle(Triplet A_, Triplet B_, Triplet C_, const Colour &c_ = Colour(0,0,255)) : \
      Geometry(c_), A(A_), B(B_), C(C_), AB(A_,B_), AC(A_,C_) {}

    const Triplet normal() const {
      return AB.cross(AC);
    }
};

struct Sphere : public Geometry{
  public:
    Triplet center;
    double radius;

  public:
    Sphere() : Geometry(), center(), radius(1) {}
    Sphere(Triplet center_, double radius_, const Colour &colour_ = Colour(0,0,255)) : \
      Geometry(colour_), center(center_), radius(radius_) {}

    const Triplet normal(const Triplet &pos) const {
      // check if p is on surface
      double dist_to_center = (pos-center).norm();
      if (dist_to_center-radius < EPS && dist_to_center-radius > -EPS )
        return (pos-center)/(pos-center).norm();
    }
};
