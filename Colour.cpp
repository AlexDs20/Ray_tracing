#include <iostream>
#include <sstream>
#include <string>
#include "math.h"
#include "Colour.h"

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
  r_ = (int16_t)strtol(R, NULL, 16);
  g_ = (int16_t)strtol(G, NULL, 16);
  b_ = (int16_t)strtol(B, NULL, 16);
}

std::string Colour::to_hex(){
  std::string hex;
  std::stringstream ss(hex);
  ss << "#" << std::hex << (r_ << 16 | g_ << 8 | b_);
  hex = ss.str();
  return hex;
}

void Colour::print_rgb(){
  std::cout << r_ << " " << g_ << " " << b_ << std::endl;
}

void Colour::print_hex(){
  std::cout << this->to_hex() << std::endl;
}

Colour Colour::operator+(const Colour &lhs){
  int16_t r = std::min(r_+lhs.r_, 255);
  int16_t g = std::min(g_+lhs.g_, 255);
  int16_t b = std::min(b_+lhs.b_, 255);
  return Colour(r, g, b);
}

Colour Colour::operator-(const Colour &lhs){
  int16_t r = std::max(r_-lhs.r_, 0);
  int16_t g = std::max(g_-lhs.g_, 0);
  int16_t b = std::max(b_-lhs.b_, 0);
  return Colour(r, g, b);
}

Colour Colour::operator* (const double &scale){
  int16_t r = std::max(std::min((int)(r_*scale), 255), 0);
  int16_t g = std::max(std::min((int)(g_*scale), 255), 0);
  int16_t b = std::max(std::min((int)(b_*scale), 255), 0);
  return Colour(r, g, b);
}

Colour Colour::operator/ (const double &scale){
  int16_t r = std::max(std::min((int)(r_/scale), 255), 0);
  int16_t g = std::max(std::min((int)(g_/scale), 255), 0);
  int16_t b = std::max(std::min((int)(b_/scale), 255), 0);
  return Colour(r, g, b);
}
