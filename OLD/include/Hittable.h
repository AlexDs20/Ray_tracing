#pragma once
#include <memory>
#include "Triplet.h"
#include "Vector3.h"
#include "Colour.h"
#include "Material.h"

class Material;

struct hit_record{
  public:
    Triplet p;    // position
    Triplet n;    // normal
    std::shared_ptr<Material> mat_ptr;
    double t;     // where along the ray the hit is
    bool front;  // front face

  public:
    hit_record() : p(), n(), t(0) {}
    hit_record(Triplet p_, Triplet n_, double t_);
    void set(const Vector3 &ray_, double t_, const Triplet &normal_, std::shared_ptr<Material> m_);
};

class Hittable{
  public:
    virtual bool intersect(const Vector3 &ray_, double t_min_, double t_max_, hit_record &rec_) const = 0;
};

struct Triangle : public Hittable{
  private:
    const Triplet A, B, C;
    const Vector3 AB, AC;
    std::shared_ptr<Material> mat_ptr;

  public:
    // Constructors
    Triangle();
    Triangle(const Triplet &A_, const Triplet &B_, const Triplet &C_, std::shared_ptr<Material> m_);

    // Methods
    Triplet normal() const;
    virtual bool intersect(const Vector3 &ray_, double t_min_, double t_max_, hit_record &rec_) const;
    const Colour get_colour(const Triplet &p_) const;
};

struct Sphere : public Hittable{
  private:
    const Triplet center;
    const double radius;
    std::shared_ptr<Material> mat_ptr;

  public:
    // Constructors
    Sphere();
    Sphere(const Triplet &center_, const double &radius_, std::shared_ptr<Material> m_);

    // Methods
    const Triplet normal(const Triplet &pos_) const;
    virtual bool intersect(const Vector3 &ray_, double t_min_, double t_max_, hit_record &rec_) const;
    const Colour get_colour(const Triplet &p_) const;
};
