#include <iostream>
#include <math.h>
#include "Triplet.h"
#include "Vector3.h"
#include "Geometries.h"

#include "Define.h"

bool intersection_triangle(const Vector3 &ray, const Triangle &tri, double &t){

  Triplet N = tri.normal();
  double D = ray.dot(N);
  if (D > -EPS && D < EPS)
    return 0;

  double invD = 1.0/D;
  Triplet AS = ray.start - tri.A;

  double alpha = ray.dot(AS.cross(tri.AC))*invD;
  if (alpha < 0 || alpha > 1)
    return 0;
  double beta = ray.dot(tri.AB.cross(AS))*invD;
  if (beta < 0 || beta > 1 || alpha+beta>1)
    return 0;

  t = -AS.dot(N)*invD;
  return 1;
}

bool intersection_sphere(const Vector3 &ray, const Sphere &s, double &t){

  Vector3 CS(s.center, ray.start);
  double a = ray.norm2();
  double b = 2.0 * CS.dot(ray);
  double c = CS.norm2() - s.radius*s.radius;
  double delta = b*b - 4*a*c;

//  double a = ray.norm2();
//  double b = 2*ray.dot(CS);
//  double c = CS.norm2() - s.radius*s.radius;

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

