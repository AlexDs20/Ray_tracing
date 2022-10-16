#pragma once
#include <string>
#include <iostream>
#include <math.h>

struct Colour{
  private:
    double r_, g_, b_;

  public:
    Colour() : r_(0), g_(0), b_(0) { }
    Colour(const double &R, const double &G, const double &B);
    Colour(std::string);
    Colour(const char*);

    double r() const { return r_; }
    double g() const { return g_; }
    double b() const { return b_; }
    double& r() { return r_; }
    double& g() { return g_; }
    double& b() { return b_; }

    std::string to_hex() const;

    void print_rgb() const;
    void print_hex() const;
    void power(double exponent);

    // Operators
    Colour operator+ (const Colour&) const;
    Colour operator- (const Colour&) const;
    Colour operator* (const Colour&) const;
    Colour operator* (const double&) const;
    Colour operator/ (const double&) const;

    Colour operator+= (const Colour&);
    Colour operator-= (const Colour&);
    Colour operator*= (const Colour&);
    Colour operator*= (const double&);
    Colour operator/= (const double&);
    bool operator!= (const Colour&) const;
    bool operator== (const Colour&) const;
};

inline Colour operator*(const double &lhs, const Colour &c){
  return c*lhs;
}

inline std::ostream& operator<< (std::ostream& out, const Colour &c) {
  return out << "(" << c.r() << "," << c.g() << "," << c.b() << ")";
}
