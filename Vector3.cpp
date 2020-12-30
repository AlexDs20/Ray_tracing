#include <iostream>
#include <math.h>
#include "triplet.h"
#include "Vector3.h"

Vector3::Vector3(){
  this->dir_ = triplet();
  this->start_ = triplet();
}

Vector3::Vector3(const triplet &vec){
  this->dir_ = vec;
  this->start_ = triplet();
}

Vector3::Vector3(const triplet &start, const triplet &end){
  this->start_ = start;
  this->end_ = end;
  this->dir_ = this->end_ - this->start_;
}

Vector3 Vector3::operator+ (const Vector3 &rhs){
  Vector3 v_out(this->dir_+rhs.dir_);
  v_out.set_start(this->start_);
  v_out.end_ = triplet(v_out.end());
  return v_out;
}

Vector3 Vector3::operator- (const Vector3 &rhs){
  Vector3 v_out(this->dir_+rhs.dir_);
  v_out.set_start(this->start_);
  v_out.end_ = triplet(v_out.end());
  return v_out;
}

Vector3 Vector3::operator* (const double &rhs){
  Vector3 v_out(this->dir_*rhs);
  v_out.set_start(this->start_);
  v_out.end_ = triplet(v_out.end());
  return v_out;
}

Vector3 Vector3::operator/ (const double &rhs){
  Vector3 v_out(this->dir_/rhs);
  v_out.set_start(this->start_);
  v_out.end_ = triplet(v_out.end());
  return v_out;
}

void Vector3::set_start(const triplet& start){
  this->start_ = start;
}


triplet Vector3::end(){
  return this->start_+this->dir_;
}

triplet Vector3::unit(){
  return this->dir_/this->norm();
}

double Vector3::norm(){
  return this->dir_.norm();
}

double Vector3::norm2(){
  return this->dir_.norm2();
}

double Vector3::dot(const Vector3 &vec){
  return this->dir_.dot(vec.dir_);
}

Vector3 Vector3::cross(const Vector3 &vec){
  return Vector3(this->dir_.cross(vec.dir_));
}

void Vector3::print(){
  std::cout << "The vector starts at: " << std::endl;
  std::cout << "(" << start_.x << "," << start_.y << "," << start_.z << ")" << std::endl;
  std::cout << "The vector direction is: " << std::endl;
  std::cout << "(" << dir_.x << "," << dir_.y << "," << dir_.z << ")" << std::endl;

}

