#include <iostream>
#include <math.h>
#include "Triplet.h"

Triplet Triplet::operator+ (const Triplet& rhs) const {
  return Triplet(t[0]+rhs.x(), t[1]+rhs.y(), t[2]+rhs.z());
}

Triplet Triplet::operator- (const Triplet& rhs) const {
  return Triplet(t[0]-rhs.x(), t[1]-rhs.y(), t[2]-rhs.z());
}

Triplet Triplet::operator* (const double& rhs) const {
  return Triplet(t[0]*rhs, t[1]*rhs, t[2]*rhs);
}

Triplet Triplet::operator/ (const double& rhs) const {
  return Triplet(t[0]/rhs, t[1]/rhs, t[2]/rhs);
}

Triplet Triplet::operator+= (const Triplet &rhs) {
  t[0] += rhs.x();
  t[1] += rhs.y();
  t[2] += rhs.z();
  return *this;
}

Triplet Triplet::operator-= (const Triplet &rhs) {
  t[0] -= rhs.x();
  t[1] -= rhs.y();
  t[2] -= rhs.z();
  return *this;
}

Triplet Triplet::operator*= (const double &rhs) {
  t[0] *= rhs;
  t[1] *= rhs;
  t[2] *= rhs;
  return *this;
}

Triplet Triplet::operator/= (const double &rhs) {
  t[0] /= rhs;
  t[1] /= rhs;
  t[2] /= rhs;
  return *this;
}

bool Triplet::operator==(const Triplet &rhs) const {
  return t[0] == rhs.x() && t[1] == rhs.y() && t[2] == rhs.z();
}

bool Triplet::operator!=(const Triplet &rhs) const {
  return t[0] != rhs.x() || t[1] != rhs.y() || t[2] != rhs.z();
}

Triplet Triplet::operator- () const {
  return Triplet(-t[0], -t[1], -t[2]);
}

double Triplet::operator[] (unsigned int i) const {
  return t[i];
}

double& Triplet::operator[] (unsigned int i) {
  return t[i];
}

double Triplet::norm2() const {
  return t[0]*t[0] + t[1]*t[1] + t[2]*t[2];
}

double Triplet::norm() const {
  return sqrt(this->norm2());
}

Triplet Triplet::unit() const {
  return *this/(this->norm());
}

double Triplet::dot(const Triplet &rhs) const {
  return t[0]*rhs.x() + t[1]*rhs.y() + t[2]*rhs.z();
}

Triplet Triplet::cross(const Triplet &rhs) const {
  return Triplet( \
    t[1] * rhs.z() - t[2] * rhs.y(), \
    t[2] * rhs.x() - t[0] * rhs.z(), \
    t[0] * rhs.y() - t[1] * rhs.x());
}
