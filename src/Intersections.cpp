#include <iostream>
#include "Triplet.h"
#include "Vector3.h"
#include "Triangle.h"

#define EPS 0.00000000000001

bool intersection_triangle(const Vector3 &ray, const Triangle &tri, double &t){

  Triplet N = tri.normal();
  double D = ray.dot(N);
  if (D > -EPS && D < EPS)
    return 0;

  Triplet AS = ray.start - tri.A;

  double alpha = ray.dot(AS.cross(tri.AC))/D;
  if (alpha < 0 || alpha > 1)
    return 0;
  double beta = ray.dot((tri.AB).cross(AS))/D;
  if (beta < 0 || beta > 1 || alpha+beta>1)
    return 0;

  t = -AS.dot(N)/D;
  return 1;
}
