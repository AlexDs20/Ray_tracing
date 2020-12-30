#include <iostream>
#include <math.h>
#include "triplet.h"

triplet::triplet(){
  x = 0;
  y = 0;
  z = 0;
}

triplet::triplet(const double &a, const double &b, const double &c){
  x = a;
  y = b;
  z = c;
}

void triplet::print(){
  std::cout << "(" << x << "," << y << "," << z << ")" << std::endl;
}

triplet triplet::operator+ (const triplet& rhs){
  return triplet(x+rhs.x, y+rhs.y, z+rhs.z);
}

triplet triplet::operator- (const triplet& rhs){
  return triplet(x-rhs.x, y-rhs.y, z-rhs.z);
}

triplet triplet::operator* (const double& rhs){
  return triplet(x*rhs, y*rhs, z*rhs);
}

triplet triplet::operator/ (const double& rhs){
  return triplet(x/rhs, y/rhs, z/rhs);
}

double triplet::norm2(){
  return x*x + y*y + z*z;
}

double triplet::norm(){
  return sqrt(this->norm2());
}

double triplet::dot(const triplet &rhs){
  return x*rhs.x + y*rhs.y + z*rhs.z;
}

triplet triplet::cross(const triplet &rhs){
  return triplet( \
    y * rhs.z - z * rhs.y, \
    z * rhs.x - x * rhs.z, \
    x * rhs.y - y * rhs.x);
}
