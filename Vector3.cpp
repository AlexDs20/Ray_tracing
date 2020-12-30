#include <iostream>
#include <math.h>

struct triplet{
  double x, y, z;

  // Constructors
  triplet(){x=0;y=0;z=0;};
  triplet(double a, double b, double c){
    x = a;
    y = b;
    z = c;
  };
  void print(){
    std::cout << "(" << x << "," << y << "," << z << ")" << std::endl;
  }
};


class Vector3{
    triplet start_, dir_, end_;

  public:
    // Constructors
    Vector3();         // Given the direction
    Vector3(const triplet&);         // Given the direction
    Vector3(const triplet&, const triplet&);   // Given the start and end point

    // Methods
    triplet head();
    double norm();
    double norm2();
    double dot(const Vector3&);
    Vector3 cross(const Vector3&);
    void print();

};

Vector3::Vector3(){
  this->dir_.x = 0;
  this->dir_.y = 0;
  this->dir_.z = 0;
  this->start_.x = 0;
  this->start_.y = 0;
  this->start_.z = 0;
}

Vector3::Vector3(const triplet &vec){
  this->dir_.x = vec.x;
  this->dir_.y = vec.y;
  this->dir_.z = vec.z;
  this->start_.x = 0;
  this->start_.y = 0;
  this->start_.z = 0;
}

Vector3::Vector3(const triplet &start, const triplet &end){
  this->start_.x = start.x;
  this->dir_.x = end.x - start.x;
  this->end_.x = end.x;

  this->start_.y = start.y;
  this->dir_.y = end.y - start.y;
  this->end_.y = end.y;

  this->start_.z = start.z;
  this->dir_.z = end.z - start.z;
  this->end_.z = end.z;
}

triplet Vector3::head(){
  triplet out;
  out.x = this->start_.x + this->dir_.x;
  out.y = this->start_.y + this->dir_.y;
  out.z = this->start_.z + this->dir_.z;
  return out;
}

double Vector3::norm(){
  return sqrt(this->norm());
}

double Vector3::norm2(){
  return dir_.x*dir_.x + dir_.y*dir_.y + dir_.z*dir_.z;
}

double Vector3::dot(const Vector3 &vec){
  return vec.dir_.x*this->dir_.x + vec.dir_.y*this->dir_.y + vec.dir_.z*this->dir_.z;
}

Vector3 Vector3::cross(const Vector3 &vec){
  Vector3 out;
  out.dir_.x = this->dir_.y * vec.dir_.z - this->dir_.z * vec.dir_.y;
  out.dir_.y = this->dir_.z * vec.dir_.x - this->dir_.x * vec.dir_.z;
  out.dir_.z = this->dir_.x * vec.dir_.y - this->dir_.y * vec.dir_.x;
  return out;
}

void Vector3::print(){
  std::cout << "The vector starts at: " << std::endl;
  std::cout << "(" << start_.x << "," << start_.y << "," << start_.z << ")" << std::endl;
  std::cout << "The vector direction is: " << std::endl;
  std::cout << "(" << dir_.x << "," << dir_.y << "," << dir_.z << ")" << std::endl;

}

int main()
{
  //-------------------
  triplet dir(1, 2, 3);
  Vector3 vec(dir);


  //-------------------
  triplet start(1,1,1);
  triplet end(2,2,2);
  Vector3 vec_new(start, end);
  vec_new.print();
  vec_new.head().print();

  (vec.cross(vec_new)).print();
  //-------------------

  return 0;
}
