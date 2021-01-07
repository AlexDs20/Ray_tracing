#include "Define.h"

#include <math.h>
#include "Geometries.h"

// Geometry class
Geometry::Geometry() : colour(Colour(0,0,255)) {}

Geometry::Geometry(const Colour &colour_) : colour(colour_) {}

void Geometry::set_colour(const Colour &c_){
  colour = c_;
}

const Colour Geometry::get_colour() const{
  return colour;
}


// Triangle class

Triangle::Triangle() : Geometry(), A(), B(), C() {}
Triangle::Triangle(const Triplet &A_, const Triplet &B_, const Triplet &C_, const Colour &colour_) \
          : Geometry(colour_), A(A_), B(B_), C(C_), AB(A_,B_), AC(A_,C_) {}

Triplet Triangle::normal() const {
  return AB.cross(AC).unit();
}

bool Triangle::intersect(const Vector3 &ray, double &t) const {
  Triplet N = this->AB.cross(this->AC);
  double D = ray.dot(N);

  if (D > -EPS && D < EPS)
    return 0;

  double invD = 1.0/D;
  Triplet AS = ray.start - this->A;

  double alpha = ray.dot(AS.cross(this->AC))*invD;
  if (alpha < 0 || alpha > 1)
    return 0;
  double beta = ray.dot(this->AB.cross(AS))*invD;
  if (beta < 0 || beta > 1 || alpha+beta>1)
    return 0;

  t = -AS.dot(N)*invD;
  return 1;
}

// Sphere class
Sphere::Sphere() : Geometry(), center(), radius(1) {}
Sphere::Sphere(const Triplet &center_, const double &radius_, const Colour &colour_) \
          : Geometry(colour_), center(center_), radius(radius_) {}

const Triplet Sphere::normal(const Triplet &pos) const {
  // check if p is on surface
  double dist_to_center = (pos-center).norm();
  if (dist_to_center-radius < EPS && dist_to_center-radius > -EPS )
    return (pos-center).unit();
  return Triplet();
}

bool Sphere::intersect(const Vector3 &ray, double &t) const {
  Vector3 CS(this->center, ray.start);
  double a = ray.norm2();
  double b = 2.0 * CS.dot(ray);
  double c = CS.norm2() - this->radius*this->radius;
  double delta = b*b - 4*a*c;

  if (delta < 0)
    return 0;

  double t1 = (-b+sqrt(delta))/(2*a);
  double t2 = (-b-sqrt(delta))/(2*a);

  if ( (ray(t1)-ray.start).norm2() < (ray(t2)-ray.start).norm2() )
    t = t1;
  else
    t = t2;
  return 1;
}

const Colour Sphere::get_colour(const Triplet &p_) const {
  Triplet N = this->normal(p_);

  return 0.5*Colour((int)255*N.x(),(int)255*N.y(),(int)255*N.z());
}
