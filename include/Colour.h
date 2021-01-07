#pragma once
#include <string>
#include <iostream>
#include <math.h>

struct Colour{
  private:
    std::uint16_t r_, g_, b_;

  public:
    Colour() : r_(0), g_(0), b_(0) { }
    Colour(const short &R, const short &G, const short &B);
    Colour(std::string);
    Colour(const char*);

    std::uint16_t r() const { return r_; }
    std::uint16_t g() const { return g_; }
    std::uint16_t b() const { return b_; }
    std::uint16_t& r() { return r_; }
    std::uint16_t& g() { return g_; }
    std::uint16_t& b() { return b_; }

    std::string to_hex() const;

    void print_rgb() const;
    void print_hex() const;

    // Operators
    Colour operator+ (const Colour&) const;
    Colour operator- (const Colour&) const;
    Colour operator* (const double&) const;
    Colour operator/ (const double&) const;

    Colour operator+= (const Colour&);
    Colour operator-= (const Colour&);
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
