#include <iostream>

#include "Vector3.h"
#include "Triplet.h"
#include "Colour.h"
#include "Image.h"
#include "Triangle.h"

#include "Intersections.h"

int main()
{
if (0) {
  //-------------------
  // Null vector
  Triplet vec0;
  std::cout << vec0 << std::endl;
  // Simply the direction
  Triplet vec1(1,2,3);
  // Given start (1,2,3) and end (3,4,5) points
  Vector3 vec2(1,2,3, 3, 3, 3);
  std::cout << vec1 << std::endl;
  vec1 += vec2;
  std::cout << vec1 << std::endl;

  std::cout << vec2.start << std::endl;
  std::cout << vec2.end() << std::endl;
  std::cout << vec1.norm() << std::endl;
  std::cout << vec1.norm2() << std::endl;

  std::cout << "Size of a triplet: " << sizeof(Triplet) << " bytes." << std::endl;
  std::cout << "Size of a Vector3: " << sizeof(Vector3) << " bytes." << std::endl;

  std::cout << vec1.cross(vec2) << std::endl;
  std::cout << vec1.dot(vec2) << std::endl;
  //-------------------
  std::cout << vec1 + vec2 << std::endl;
  std::cout << vec1 - vec2 << std::endl;
  std::cout << vec1 * 2 << std::endl;
  std::cout << vec1 / 2 << std::endl;

  std::cout << vec1.unit() << std::endl;;
}

if (0){
  //-------------------
  Colour color1(170, 21, 33);
  color1.print_rgb();
  color1.print_hex();

  Colour color2(150, 0, 50);
  (color1 + color2).print_rgb();
  (color1 - color2).print_rgb();
  (2*color1).print_rgb();
  (color1/2).print_rgb();
}

if (0){
  Colour red(255,255,0);
  Image img(1024, 768, red);
  img.save_PPM("Image.ppm");
}

if (1){
  Vector3 ray(0.2,0.2,10,0.2,0.2,9);
  Triplet A(0,0,0);
  Triplet B(1,0,0);
  Triplet C(0,1,0);
  const Triangle tri(A,B,C);
  double t=0;
  intersection_triangle(ray, tri, t);
  std::cout << ray(t) << std::endl;
}

  return 0;
}
