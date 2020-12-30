#pragma once

#include "triplet.h"

class Vector3{
    triplet start_, dir_, end_;

  public:
    // Constructors
    Vector3();         // Given the direction
    Vector3(const triplet&);         // Given the direction
    Vector3(const triplet&, const triplet&);   // Given the start and end point

    // Operators
    Vector3 operator+ (const Vector3&);         // Add 2 vectors
    Vector3 operator- (const Vector3&);         // Substract 2 vectors
    Vector3 operator* (const double&);          // Product by scalar
    Vector3 operator/ (const double&);          // Division by scalar

    // Methods
    void set_start(const triplet&);
    triplet end();
    triplet unit();
    double norm();
    double norm2();
    double dot(const Vector3&);
    Vector3 cross(const Vector3&);
    void print();

};
