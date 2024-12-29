#include "Utilities.h"

#include <cmath>
#include <cstdlib>
#include <vector>
#include "Define.h"
#include "Colour.h"
#include "Vector3.h"
#include "Triplet.h"
#include "Hittable.h"

void rand_in_disc(double r, double *coord){
  // Return the coordinates of a uniformly distributed random point in disc of radius r
  // point = u*e_u + v*e_v
  // coord[0] = u, coord[1] = v
  double phi = (2*my_rand() - 1)*PI;
  double a = sqrt(r * my_rand());

  coord[0] = a*cos(phi);
  coord[1] = a*sin(phi);
}

Triplet random_vector_half_unit_sphere(const Triplet &normal_){
  double phi = 2*PI*my_rand();
  double theta = acos(1 - my_rand());
  Triplet v(sin(theta)*cos(phi), sin(theta)*sin(phi), cos(theta));

  Triplet z(0,0,1);

  if (z == normal_.unit())
    return v;

  Triplet k = z.cross(normal_.unit());
  theta = asin(k.norm());
  v = v*cos(theta) + k.cross(v) * sin(theta) + k * (k.dot(v)) * (1-cos(theta));

  return v;
}

Triplet random_vector_sphere(double radius_){
  // Returns a random point in a sphere of radius_
  // with uniform distribution
  // Solution from:
  // https://math.stackexchange.com/questions/87230/picking-random-points-in-the-volume-of-sphere-with-uniform-probability/87238#87238
  double u = radius_ * pow(my_rand(), 1/3);
  Triplet R(my_rand(), my_rand(), my_rand());
  R = 2*R - Triplet(1,1,1);
  R = R.unit();
  return u*R;
}

Colour calculate_colour(const Vector3 &ray_, const Hittable &scene_, int depth_){
  if (depth_<=0)
    return Colour(0,0,0);

  hit_record rec;
  if (scene_.intersect(ray_, EPS, infty, rec)){
    Vector3 scattered;
    Colour attenuation;
    if (rec.mat_ptr->scatter(ray_, rec, attenuation, scattered))
      return attenuation*calculate_colour(scattered, scene_, depth_-1);
    return Colour(0,0,0);
  }
  double x = 0.5*(ray_.unit().y() + 1.0);
  return (1-x)*Colour(1.,1.,1.) + x * Colour(0.5, 0.8, 1.);
}

void gamma_correction(Colour &colour_, double gamma_){
  colour_.power(1./gamma_);
}

double my_rand(){
  return (double)rand()/RAND_MAX;
}

double my_rand(double low, double up){
  return (up-low)*rand()/RAND_MAX + low;
}

double dot(const Vector3 &v1, const Triplet &v2){
  return v1.t[0]*v2.x()+v1.t[1]*v2.y()+v1.t[2]*v2.z();
}

Vector3 reflect(const Vector3 &ray_, const Triplet &n_){
  return ray_ - 2*dot(ray_,n_) * n_;
}

Vector3 refract(const Vector3 &ray_, const Triplet &n_, double n_over_n_in){
  Vector3 r_perp = n_over_n_in * ray_ - n_over_n_in * ray_.dot(n_) * n_;
  Vector3 r_para = copysign(1, ray_.dot(n_)) * sqrt( 1 - r_perp.norm2() ) * n_;
  return r_perp + r_para;
}
