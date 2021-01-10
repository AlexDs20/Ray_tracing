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
  // i.e. V.dot(N) = 0
  normal_ = Triplet((double)rand()/RAND_MAX, (double)rand()/RAND_MAX, \
                    (double)rand()/RAND_MAX);
  normal_ = normal_.unit();
  double vz = -(normal_.x() + normal_.y())/normal_.z();
  Triplet k(1, 1, vz);
  k = k.unit();

  // Generate the rotation angles
  double phi = (double)2*PI*rand()/RAND_MAX;
  double theta = (double)0.5*rand()/RAND_MAX;

  // Start with the theta rotation around k
  // using Rodrigues formula: https://en.wikipedia.org/wiki/Rodrigues%27_rotation_formula
  Triplet N_rot;
  N_rot = normal_*cos(theta) + (k.cross(normal_))*sin(theta) + k*(k.dot(normal_))*(1-cos(theta));
  // Now the phi rotation around the normal_
  N_rot = N_rot*cos(phi) + (normal_.cross(N_rot))*sin(phi) + normal_*(normal_.dot(N_rot))*(1-cos(phi));

  return N_rot;
}

Colour calculate_colour(const Vector3 &ray_, const Hittable &scene_, int depth_){
  if (depth_<=0)
    return Colour(0,0,0);

  hit_record rec;
  if (scene_.intersect(ray_, EPS, infty, rec)){
    Triplet random_unit = random_vector_unit_half_sphere(rec.n);
    Vector3 reflected_ray(rec.p, rec.p+random_unit);
    return 0.5*calculate_colour(reflected_ray, scene_, depth_-1);
  }
  double x = 0.5*(ray_.unit().y() + 1.0);
  return (1-x)*Colour(255,255,255) + x * Colour(128, 200, 255);
}

void gamma_correction(Colour &colour_, double gamma_){
  colour_.power(1./gamma_);
}

