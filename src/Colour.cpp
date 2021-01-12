#include <iostream>
#include <sstream>
#include <string>
#include "math.h"
#include "Colour.h"

Colour::Colour(const double &R, const double &G, const double &B) : \
  r_(std::max(std::min(R, 1.), 0.)), \
  g_(std::max(std::min(G, 1.), 0.)), \
  b_(std::max(std::min(B, 1.), 0.)) { }

Colour::Colour(std::string hex) {
  r_ = std::stoi(hex.substr(1, 2), 0, 16)/255;
  g_ = std::stoi(hex.substr(3, 2), 0, 16)/255;
  b_ = std::stoi(hex.substr(5, 2), 0, 16)/255;
}

Colour::Colour(const char* hex){
  // TODO: This is not memory safe!
  //  Check the size and if hex[0] = '#'
  char R[] = {hex[1], hex[2], '\0'};
  char G[] = {hex[3], hex[4], '\0'};
  char B[] = {hex[5], hex[6], '\0'};
  r_ = (double)strtol(R, NULL, 16)/255;
  g_ = (double)strtol(G, NULL, 16)/255;
  b_ = (double)strtol(B, NULL, 16)/255;
}

std::string Colour::to_hex() const{
  std::string hex;
  std::stringstream ss(hex);
  ss << "#" << std::hex << ((int)(r_*255) << 16 | (int)(g_*255) << 8 | (int)(b_*255));
  hex = ss.str();
  return hex;
}

void Colour::print_rgb() const{
  std::cout << r_ << " " << g_ << " " << b_ << std::endl;
}

void Colour::print_hex() const{
  std::cout << this->to_hex() << std::endl;
}

void Colour::power(double exp_){
  r_ = std::min(std::max( pow(r_, exp_), 0.), 1.);
  g_ = std::min(std::max( pow(g_, exp_), 0.), 1.);
  b_ = std::min(std::max( pow(b_, exp_), 0.), 1.);
}

Colour Colour::operator+(const Colour &lhs) const{
  double r = std::min(r_+lhs.r(), 1.);
  double g = std::min(g_+lhs.g(), 1.);
  double b = std::min(b_+lhs.b(), 1.);
  return Colour(r, g, b);
}

Colour Colour::operator-(const Colour &lhs) const{
  double r = std::max(r_-lhs.r(), 0.);
  double g = std::max(g_-lhs.g(), 0.);
  double b = std::max(b_-lhs.b(), 0.);
  return Colour(r, g, b);
}

Colour Colour::operator* (const double &scale) const{
  double r = std::max(std::min(r_*scale, 1.), 0.);
  double g = std::max(std::min(g_*scale, 1.), 0.);
  double b = std::max(std::min(b_*scale, 1.), 0.);
  return Colour(r, g, b);
}

Colour Colour::operator* (const Colour &c2) const{
  double r = std::max(std::min(r_*c2.r_, 1.), 0.);
  double g = std::max(std::min(g_*c2.g_, 1.), 0.);
  double b = std::max(std::min(b_*c2.b_, 1.), 0.);
  return Colour(r, g, b);
}

Colour Colour::operator/ (const double &scale) const{
  double r = std::max(std::min(r_/scale, 1.), 0.);
  double g = std::max(std::min(g_/scale, 1.), 0.);
  double b = std::max(std::min(b_/scale, 1.), 0.);
  return Colour(r, g, b);
}

Colour Colour::operator+= (const Colour &c2){
  r_ += c2.r();
  g_ += c2.g();
  b_ += c2.b();
  r_ = std::min(r_, 1.);
  g_ = std::min(g_, 1.);
  b_ = std::min(b_, 1.);
  return *this;
}

Colour Colour::operator-= (const Colour &c2){
  r_ -= c2.r();
  g_ -= c2.g();
  b_ -= c2.b();
  r_ = std::max(r_, 0.);
  g_ = std::max(g_, 0.);
  b_ = std::max(b_, 0.);
  return *this;
}

Colour Colour::operator*= (const double &val){
  r_ *= val;
  g_ *= val;
  b_ *= val;
  r_ = std::max(std::min(r_, 1.), 0.);
  g_ = std::max(std::min(g_, 1.), 0.);
  b_ = std::max(std::min(b_, 1.), 0.);
  return *this;
}

Colour Colour::operator*= (const Colour &c2){
  r_ *= c2.r_;
  g_ *= c2.g_;
  b_ *= c2.b_;
  r_ = std::max(std::min(r_, 1.), 0.);
  g_ = std::max(std::min(g_, 1.), 0.);
  b_ = std::max(std::min(b_, 1.), 0.);
  return *this;
}

Colour Colour::operator/= (const double &val){
  r_ /= val;
  g_ /= val;
  b_ /= val;
  r_ = std::max(std::min(r_, 1.), 0.);
  g_ = std::max(std::min(g_, 1.), 0.);
  b_ = std::max(std::min(b_, 1.), 0.);
  return *this;
}

bool Colour::operator!= (const Colour &c2) const{
  return r_!=c2.r() || g_!=c2.g() || b_!=c2.b();
}

bool Colour::operator== (const Colour &c2) const{
  return r_==c2.r() && g_==c2.g() && b_==c2.b();
}

