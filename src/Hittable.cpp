#include <math.h>

#include <memory>
#include "Define.h"
#include "Hittable.h"
#include "Material.h"

// Hit_record struct
hit_record::hit_record(Triplet p_, Triplet n_, double t_) : p(p_), n(n_), t(t_) {}

void hit_record::set(const Vector3 &ray_, double t_, const Triplet &normal_, std::shared_ptr<Material> m_){
  n = normal_;
  t = t_;
  p = ray_(t);
  front = ray_.dot(n) < 0;
  mat_ptr = m_;
}


// Triangle class

Triangle::Triangle() : A(), B(), C() {}
Triangle::Triangle(const Triplet &A_, const Triplet &B_, const Triplet &C_, std::shared_ptr<Material> m_) \
          : A(A_), B(B_), C(C_), AB(A_,B_), AC(A_,C_), mat_ptr(m_) {}

Triplet Triangle::normal() const {
  return AB.cross(AC).unit();
}

bool Triangle::intersect(const Vector3 &ray_, double t_min_, double t_max_, hit_record &rec_) const {
  Triplet N = this->AB.cross(this->AC);
  double D = ray_.dot(N);

  if (D > -EPS && D < EPS)
    return 0;

  double invD = 1.0/D;
  Triplet AS = ray_.start - this->A;

  double alpha = ray_.dot(AS.cross(this->AC))*invD;
  if (alpha < 0 || alpha > 1)
    return 0;
  double beta = ray_.dot(this->AB.cross(AS))*invD;
  if (beta < 0 || beta > 1 || alpha+beta>1)
    return 0;

  double t = -AS.dot(N)*invD;

  if (t<t_min_ || t>t_max_)
    return 0;

  rec_.set(ray_, t, normal(), mat_ptr);
  return 1;
}

const Colour Triangle::get_colour(const Triplet &p_) const {
  Triplet N = this->normal() + p_.unit();

  return 0.5*Colour((int)255*N.x(),(int)255*N.y(),(int)255*N.z());
}

// Sphere class
Sphere::Sphere() : center(), radius(1) {}
Sphere::Sphere(const Triplet &center_, const double &radius_, std::shared_ptr<Material> m_) \
          : center(center_), radius(radius_), mat_ptr(m_) {}

const Triplet Sphere::normal(const Triplet &pos) const {
  // check if p is on surface
  double dist_to_center = (pos-center).norm();
  if (dist_to_center-radius < EPS && dist_to_center-radius > -EPS )
    return (pos-center).unit();
  return Triplet();
}

bool Sphere::intersect(const Vector3 &ray_, double t_min_, double t_max_, hit_record &rec_) const {
  Vector3 CS(this->center, ray_.start);
  double a = ray_.norm2();
  double b_half = CS.dot(ray_);
  double c = CS.norm2() - this->radius*this->radius;
  double delta = b_half*b_half - a*c;

  if (delta < 0)
    return 0;

  double t = (-b_half-sqrt(delta))/a;

  if ( t<t_min_ || t>t_max_ ){
    t = (-b_half+sqrt(delta))/a;
    if ( t<t_min_ || t>t_max_ )
      return 0;
  }

  rec_.set(ray_, t, normal(ray_(t)), mat_ptr);
  return 1;
}

const Colour Sphere::get_colour(const Triplet &p_) const {
  Triplet N = this->normal(p_);

  return 0.5*Colour((int)255*N.x(),(int)255*N.y(),(int)255*N.z());
}
