#include "Hittable_list.h"
#include "Hittable.h"

#include <memory>

Hittable_list::Hittable_list(std::shared_ptr<Hittable> obj_){
  add(obj_);
}

void Hittable_list::clear(){
  objects.clear();
}

void Hittable_list::add(std::shared_ptr<Hittable> obj_){
  objects.push_back(obj_);
}

bool Hittable_list::intersect(const Vector3 &ray_, double t_min_, double t_max_, hit_record &rec_) const {
  hit_record rec_tmp;
  bool hit_smth = false;
  double closest_t = t_max_;

  for (const auto &obj : objects){
    if (obj->intersect(ray_, t_min_, closest_t, rec_tmp)){
      hit_smth = true;
      closest_t = rec_tmp.t;
      rec_ = rec_tmp;
    }
  }

  return hit_smth;
}
