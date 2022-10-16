#pragma once
#include <fstream>

class Triplet{
  public:
    // Coordinates
    double t[3];

    // Constructors
    Triplet() : t{0,0,0} {}
    Triplet(double x_, double y_, double z_) : t{x_, y_, z_} {}

    double x() const { return t[0]; }
    double y() const { return t[1]; }
    double z() const { return t[2]; }

    double& x() { return t[0]; }
    double& y() { return t[1]; }
    double& z() { return t[2]; }


    // Operators
    Triplet operator+ (const Triplet&) const;         // Add 2 Triplets
    Triplet operator- (const Triplet&) const;         // Substract 2 Triplets
    Triplet operator* (const double&) const;          // Product by scalar
    Triplet operator/ (const double&) const;          // Division by scalar

    Triplet operator+= (const Triplet &);
    Triplet operator-= (const Triplet &);
    Triplet operator*= (const double &);
    Triplet operator/= (const double &);
    bool operator== (const Triplet &) const;
    bool operator!= (const Triplet &) const;

    Triplet operator- () const;
    double operator[] (unsigned int) const;
    double& operator[] (unsigned int);

    // Methods
    double norm2() const;
    double norm() const;
    Triplet unit() const;
    double dot(const Triplet&) const;
    Triplet cross(const Triplet&) const;
};

inline Triplet operator* (const double &lhs, const Triplet &t) {
  return t*lhs;
}

inline std::ostream& operator<< (std::ostream& out, const Triplet &t) {
  return out << "(" << t.x() << "," << t.y() << "," << t.z() << ")";
}
