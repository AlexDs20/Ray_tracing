#pragma once

#include "Triplet.h"

class Vector3 : public Triplet {
  public:
    Triplet start_;

    Vector3() : Triplet(), start_(0,0,0) {};
    Vector3(double _x, double _y, double _z) : Triplet(_x, _y, _z), start_(0,0,0) {}
    Vector3(double _sx, double _sy, double _sz, double _ex, double _ey, double _ez) : \
                              Triplet (_ex-_sx, _ey-_sy, _ez-_sz), start_(_sx, _sy, _sz) {}
    Triplet end() const { return *this + start_; }
};
