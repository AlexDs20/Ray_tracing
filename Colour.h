#pragma once
#include <string>

struct Colour{
  private:
    std::int16_t r_, g_, b_;

  public:
    Colour() : r_(0), g_(0), b_(0) { }
    Colour(const short &R, const short &G, const short &B) : r_(R), g_(G), b_(B) { }
    Colour(std::string);
    Colour(const char*);

    std::string to_hex();

    void print_rgb();
    void print_hex();

    // TODO: Overload operators +=, -=, *= and /=
    Colour operator+ (const Colour&);
    Colour operator- (const Colour&);
    Colour operator* (const double&);
    Colour operator/ (const double&);
};
