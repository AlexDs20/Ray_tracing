#include <iostream>
#include "Vector3.h"
#include "triplet.h"

int main()
{
  //-------------------
  triplet dir(1, 2, 3);
  Vector3 vec(dir);
  vec.set_start(triplet(0,1,1));
  std::cout << vec.norm() << std::endl;
  std::cout << vec.norm2() << std::endl;

  //-------------------
  triplet start(1,1,1);
  triplet end(2,2,2);
  Vector3 vec_new(start, end);
  vec_new.print();
  vec_new.end().print();

  (vec.cross(vec_new)).print();
  std::cout << (vec.dot(vec_new)) << std::endl;
  //-------------------
  (vec + vec_new).print();
  (vec - vec_new).print();
  (vec * 2).print();
  (vec / 2).print();

  vec.unit().print();

  return 0;
}
