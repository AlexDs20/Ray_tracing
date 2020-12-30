#pragma once

class triplet{
  public:
    // Coordinates
    double x, y, z;

    // Constructors
    triplet();
    triplet(const double&, const double&, const double&);
    void print();

    // Operators
    triplet operator+ (const triplet&);         // Add 2 triplets
    triplet operator- (const triplet&);         // Substract 2 triplets
    triplet operator* (const double&);          // Product by scalar
    triplet operator/ (const double&);          // Division by scalar

    // Methods
    double norm2();
    double norm();
    double dot(const triplet&);
    triplet cross(const triplet&);
};
