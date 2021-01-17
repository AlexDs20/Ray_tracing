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

Metal::Metal(const Colour &c_, double fuzz_) : albedo(c_), fuzziness(fuzz_) {}

bool Metal::scatter(const Vector3 &ray_, const hit_record &rec_, Colour &attenuation_, Vector3 &scattered_) const {
  scattered_ = reflect(ray_, rec_.n);
  scattered_ = Vector3(rec_.p, rec_.p + scattered_.unit() + random_vector_sphere(this->fuzziness) );
  attenuation_ = albedo;
  return (dot(scattered_, rec_.n) > 0);
}

Dielectric::Dielectric(const Colour &c_, double ref_idx_) : albedo(c_), ref_idx(ref_idx_) {}

bool Dielectric::scatter(const Vector3 &ray_, const hit_record &rec_, Colour &attenuation_, Vector3 &scattered_) const {
  attenuation_ = albedo;
  Vector3 ray_norm = ray_.unit();
  double refraction_ratio = ray_.dot(rec_.n) < 0 ? 1/ref_idx : ref_idx;

  double sin_theta = ray_norm.cross(rec_.n).norm();

  if ( refraction_ratio*sin_theta > 1 )
    scattered_ = reflect(ray_norm, rec_.n);
  else
    scattered_ = refract(ray_norm, rec_.n, refraction_ratio);

  scattered_ = Vector3(rec_.p, rec_.p + scattered_); // + random_vector_sphere(this->fuzziness) );
  return 1;
}
