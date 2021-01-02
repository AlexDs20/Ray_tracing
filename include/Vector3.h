#pragma once

#include "Triplet.h"

class Vector3 : public Triplet {
  public:
    Triplet start;

    // Constructors
    Vector3() : Triplet(), start(0,0,0) {};
    Vector3(double _x, double _y, double _z) : Triplet(_x, _y, _z), start(0,0,0) {}
    Vector3(double _sx, double _sy, double _sz, double _ex, double _ey, double _ez) : \
                              Triplet(_ex-_sx, _ey-_sy, _ez-_sz), start(_sx, _sy, _sz) {}
    Vector3(Triplet _A, Triplet _B){
      start = _A;
      t[0] = _B.x() - _A.x();
      t[1] = _B.y() - _A.y();
      t[2] = _B.z() - _A.z();
    }

    // Operators
    Triplet operator() (const double &scale) const {
      std::cout << "Start: " << start << std::endl;
      std::cout << "Direction: " << *this << std::endl;
      return start + scale*(*this);
    }

    // Methods
    Triplet end() const { return *this + start; }
};
