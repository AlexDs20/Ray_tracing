#pragma once

#include "Triplet.h"

class Vector3 : public Triplet {
  public:
    Triplet start;

    // Constructors
    Vector3() : Triplet(), start(0,0,0) {};
    Vector3(Triplet A_) {
      t[0] = A_.x();
      t[1] = A_.y();
      t[2] = A_.z();
    }
    Vector3(double x_, double y_, double z_) : Triplet(x_, y_, z_), start(0,0,0) {}
    Vector3(double sx_, double sy_, double sz_, double ex_, double ey_, double ez_) : \
                              Triplet(ex_-sx_, ey_-sy_, ez_-sz_), start(sx_, sy_, sz_) {}
    Vector3(Triplet A_, Triplet B_){
      start = A_;
      t[0] = B_.x() - A_.x();
      t[1] = B_.y() - A_.y();
      t[2] = B_.z() - A_.z();
    }

    // Operators
    const Triplet operator() (const double &scale) const {
      return start + scale*(*this);
    }

    // Methods
    Triplet end() const { return *this + start; }
};
