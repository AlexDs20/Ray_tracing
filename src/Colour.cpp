#include <iostream>
#include <sstream>
#include <string>
#include "math.h"
#include "Colour.h"

Colour::Colour(const short &R, const short &G, const short &B) : \
  r_(std::max(std::min((int)R, 255), 0)), \
  g_(std::max(std::min((int)G, 255), 0)), \
  b_(std::max(std::min((int)B, 255), 0)) { }

Colour::Colour(std::string hex) {
  r_ = std::stoi(hex.substr(1, 2), 0, 16);
  g_ = std::stoi(hex.substr(3, 2), 0, 16);
  b_ = std::stoi(hex.substr(5, 2), 0, 16);
}

Colour::Colour(const char* hex){
  // TODO: This is not memory safe!
  //  Check the size and if hex[0] = '#'
  char R[] = {hex[1], hex[2], '\0'};
  char G[] = {hex[3], hex[4], '\0'};
  char B[] = {hex[5], hex[6], '\0'};
  r_ = (uint16_t)strtol(R, NULL, 16);
  g_ = (uint16_t)strtol(G, NULL, 16);
  b_ = (uint16_t)strtol(B, NULL, 16);
}

std::string Colour::to_hex() const{
  std::string hex;
  std::stringstream ss(hex);
  ss << "#" << std::hex << (r_ << 16 | g_ << 8 | b_);
  hex = ss.str();
  return hex;
}

void Colour::print_rgb() const{
  std::cout << r_ << " " << g_ << " " << b_ << std::endl;
}

void Colour::print_hex() const{
  std::cout << this->to_hex() << std::endl;
}

Colour Colour::operator+(const Colour &lhs) const{
  uint16_t r = std::min(r_+lhs.r(), 255);
  uint16_t g = std::min(g_+lhs.g(), 255);
  uint16_t b = std::min(b_+lhs.b(), 255);
  return Colour(r, g, b);
}

Colour Colour::operator-(const Colour &lhs) const{
  uint16_t r = std::max(r_-lhs.r(), 0);
  uint16_t g = std::max(g_-lhs.g(), 0);
  uint16_t b = std::max(b_-lhs.b(), 0);
  return Colour(r, g, b);
}

Colour Colour::operator* (const double &scale) const{
  uint16_t r = std::max(std::min((int)(r_*scale), 255), 0);
  uint16_t g = std::max(std::min((int)(g_*scale), 255), 0);
  uint16_t b = std::max(std::min((int)(b_*scale), 255), 0);
  return Colour(r, g, b);
}

Colour Colour::operator/ (const double &scale) const{
  uint16_t r = std::max(std::min((int)(r_/scale), 255), 0);
  uint16_t g = std::max(std::min((int)(g_/scale), 255), 0);
  uint16_t b = std::max(std::min((int)(b_/scale), 255), 0);
  return Colour(r, g, b);
}

Colour Colour::operator+= (const Colour &c2){
  r_ += c2.r();
  g_ += c2.g();
  b_ += c2.b();
  r_ = std::min((int)r_, 255);
  g_ = std::min((int)g_, 255);
  b_ = std::min((int)b_, 255);
  return *this;
}

Colour Colour::operator-= (const Colour &c2){
  r_ -= c2.r();
  g_ -= c2.g();
  b_ -= c2.b();
  r_ = std::max((int)r_, 0);
  g_ = std::max((int)g_, 0);
  b_ = std::max((int)b_, 0);
  return *this;
}

Colour Colour::operator*= (const double &val){
  r_ *= val;
  g_ *= val;
  b_ *= val;
  r_ = std::max(std::min((int)r_, 255), 0);
  g_ = std::max(std::min((int)g_, 255), 0);
  b_ = std::max(std::min((int)b_, 255), 0);
  return *this;
}

Colour Colour::operator/= (const double &val){
  r_ /= val;
  g_ /= val;
  b_ /= val;
  r_ = std::max(std::min((int)r_, 255), 0);
  g_ = std::max(std::min((int)g_, 255), 0);
  b_ = std::max(std::min((int)b_, 255), 0);
  return *this;
}

bool Colour::operator!= (const Colour &c2) const{
  return r_!=c2.r() || g_!=c2.g() || b_!=c2.b();
}

bool Colour::operator== (const Colour &c2) const{
  return r_==c2.r() && g_==c2.g() && b_==c2.b();
}

