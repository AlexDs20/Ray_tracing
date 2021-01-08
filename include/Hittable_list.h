#pragma once

#include <vector>
#include <memory>

#include "Hittable.h"

class Hittable_list : public Hittable{
  public:
    std::vector<std::shared_ptr<Hittable>> objects;

  public:
    Hittable_list() {};
    Hittable_list(std::shared_ptr<Hittable> object_);

    void clear();
    void add(std::shared_ptr<Hittable> object_);

    virtual bool intersect( const Vector3 &ray_, double t_min_, double t_max_, hit_record &rec_) const override;
};
