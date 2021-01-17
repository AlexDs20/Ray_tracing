#include "Utilities.h"

#include <cmath>
#include <cstdlib>
#include "Define.h"
#include "Colour.h"
#include "Vector3.h"
#include "Triplet.h"
#include "Hittable.h"


Triplet random_vector_unit_half_sphere(Triplet normal_){
  // Creates a vector perpendicular to the normal
  // i.e. k.dot(N) = 0
  // k = rand
  Triplet k(my_rand(), my_rand(), my_rand());
  if (normal_.z() != 0)
    k.z() = - (k.x()*normal_.x()+k.y()*normal_.y())/normal_.z();
  else if (normal_.y() != 0)
    k.y() = - (k.x()*normal_.x()+k.z()*normal_.z())/normal_.y();
  else if (normal_.x() != 0)
    k.x() = - (k.y()*normal_.y()+k.z()*normal_.z())/normal_.x();
  k = k.unit();

  // Generate the rotation angles
  double phi = 2*PI*my_rand();
  double theta = acos(1 - my_rand());

  // Start with the theta rotation around k
  // using Rodrigues formula: https://en.wikipedia.org/wiki/Rodrigues%27_rotation_formula
  Triplet N_rot;
  N_rot = normal_*cos(theta) + (k.cross(normal_))*sin(theta) + k*(k.dot(normal_))*(1-cos(theta));
  // Now the phi rotation around the normal_
  N_rot = N_rot*cos(phi) + (normal_.cross(N_rot))*sin(phi) + normal_*(normal_.dot(N_rot))*(1-cos(phi));

  return N_rot;
}

Triplet random_vector_sphere(double radius_){
  // Generate the rotation angles
  double phi = 2*PI*my_rand();
  double theta = acos(1 - 2*my_rand());
  double r = radius_*my_rand();

  Triplet rand_vector(sin(theta)*cos(phi), sin(theta)*sin(phi), cos(theta));
  return r*rand_vector;
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
