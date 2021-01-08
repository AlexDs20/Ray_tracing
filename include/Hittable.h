#pragma once
#include "Triplet.h"
#include "Vector3.h"
#include "Colour.h"
#include "Hittable.h"


struct hit_record{
  public:
    Triplet p;    // position
    Triplet n;    // normal
    double t;
    bool front;  // front face

  public:
    hit_record() : p(), n(), t(0) {}
    hit_record(Triplet p_, Triplet n_, double t_);
    void set(const Vector3 &ray_, double t_, const Triplet &normal_);
    void set(Triplet p_, Triplet n_, double t_);
};

class Hittable{
  private:
    Colour colour;

  public:
    // Colour
    Hittable();
    Hittable(const Colour &colour_);

    // Methods
    void set_colour(const Colour &c_);
    const Colour get_colour() const;
    virtual bool intersect(const Vector3 &ray_, double t_min_, double t_max_, hit_record &rec_) const = 0;
};

struct Triangle : public Hittable{
  private:
    const Triplet A, B, C;
    const Vector3 AB, AC;

  public:
    // Constructors
    Triangle();
    Triangle(const Triplet &A_, const Triplet &B_, const Triplet &C_, const Colour &colour_);

    // Methods
    Triplet normal() const;
    virtual bool intersect(const Vector3 &ray_, double t_min_, double t_max_, hit_record &rec_) const;
    const Colour get_colour(const Triplet &p_) const;
};

struct Sphere : public Hittable{
  private:
    const Triplet center;
    const double radius;

  public:
    // Constructors
    Sphere();
    Sphere(const Triplet &center_, const double &radius_, const Colour &colour_);

    // Methods
    const Triplet normal(const Triplet &pos_) const;
    virtual bool intersect(const Vector3 &ray_, double t_min_, double t_max_, hit_record &rec_) const;
    const Colour get_colour(const Triplet &p_) const;
};
