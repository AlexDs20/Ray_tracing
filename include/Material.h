#pragma once

#include "Hittable.h"
#include "Colour.h"

struct hit_record;

class Material {
  public:
    virtual bool scatter(const Vector3 &ray_, const hit_record &rec_, Colour &attenuation_, Vector3 &scattered_) const = 0;
};

class Lambertian : public Material {
  private:
    Colour albedo;

  public:
    Lambertian(const Colour &c_);
    virtual bool scatter(const Vector3 &ray_, const hit_record &rec_, Colour &attenuation_, Vector3 &scattered_) const override;
};

class Metal : public Material {
  private:
    Colour albedo;
    double fuzziness;

  public:
    Metal(const Colour &c_, double fuzz_);
    virtual bool scatter(const Vector3 &ray_, const hit_record &rec_, Colour &attenuation_, Vector3 &scattered_) const override;
};

class Dielectric : public Material {
  private:
    Colour albedo;
    double ref_idx;
  public:
    Dielectric(const Colour &c_, double ref_idx);
    virtual bool scatter(const Vector3 &ray_, const hit_record &rec_, Colour &attenuation_, Vector3 &scattered_) const override;
};
