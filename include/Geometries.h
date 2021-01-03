#pragma once
#include "Triplet.h"
#include "Vector3.h"
#include "Colour.h"
#include <math.h>

#include "Define.h"

struct Geometry{
  private:
    Colour colour;

  public:
    Geometry() : colour(Colour(0,0,255)) {}
    Geometry(const Colour &c_) : colour(c_) {}

    void set_colour(const Colour &c_){
      colour = c_;
    }

    const Colour get_colour() const{
      return colour;
    }

    bool intersect(const Vector3 &ray, double &t){
      std::cout << ray << " " << t << std::endl;
      return 0;
    }
};

struct Triangle : public Geometry{
  public:
    Triplet A, B, C;
    Vector3 AB, AC;

  public:
    Triangle() : Geometry(), A(), B(), C() {}
    Triangle(Triplet A_, Triplet B_, Triplet C_, const Colour &c_ = Colour(0,0,255)) : \
      Geometry(c_), A(A_), B(B_), C(C_), AB(A_,B_), AC(A_,C_) {}

    const Triplet normal() const {
      return AB.cross(AC);
    }

    bool intersect(const Vector3 &ray, double &t) const {
      Triplet N = this->normal();
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


};

struct Sphere : public Geometry{
  public:
    Triplet center;
    double radius;

  public:
    Sphere() : Geometry(), center(), radius(1) {}
    Sphere(Triplet center_, double radius_, const Colour &colour_ = Colour(0,0,255)) : \
      Geometry(colour_), center(center_), radius(radius_) {}

    const Triplet normal(const Triplet &pos) const {
      // check if p is on surface
      double dist_to_center = (pos-center).norm();
      if (dist_to_center-radius < EPS && dist_to_center-radius > -EPS )
        return (pos-center)/(pos-center).norm();
    }

    bool intersect(const Vector3 &ray, double &t) const {
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

};
