#include "Material.h"

#include "Colour.h"
#include "Triplet.h"
#include "Utilities.h"

// Lambertian
Lambertian::Lambertian(const Colour &c_) : albedo(c_) {}

bool Lambertian::scatter(const Vector3 &, const hit_record &rec_, Colour &attenuation_, Vector3 &scattered_) const {
  Triplet scatter_dir = random_vector_unit_half_sphere(rec_.n);
  scattered_ = Vector3(rec_.p, rec_.p + scatter_dir);
  attenuation_ = albedo;
  return 1;
}

Metal::Metal(const Colour &c_) : albedo(c_) {}

bool Metal::scatter(const Vector3 &ray_, const hit_record &rec_, Colour &attenuation_, Vector3 &scattered_) const {
  scattered_ = reflect(ray_, rec_.n);
  attenuation_ = albedo;
  return (dot(scattered_, rec_.n) > 0);
}
