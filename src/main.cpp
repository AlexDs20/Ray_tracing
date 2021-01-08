#include <iostream>
#include <vector>
#include <memory>
#include <string>

#include "Define.h"
#include "Vector3.h"
#include "Triplet.h"
#include "Colour.h"
#include "Image.h"
#include "Hittable.h"
#include "Hittable_list.h"
#include "Camera.h"

int main()
{

// Create the camera
Triplet cam_pos(0,0,5);
Triplet cam_look(0,0,0);
Vector3 cam_vec(cam_pos, cam_look);
Camera camera(cam_vec, 1, 1024, 768, 0.005, Colour(255,255,0));

// Create the objects
Triplet A(0,0,0);
Triplet B(3,0,0);
Triplet C(0,3,0);
Triangle tri(A,B,C,Colour(255,255,255));

Triplet c(0,0,0);
Sphere sph(c, 3, Colour(0,255,0));

//  Create the scene of objs
Hittable_list scene;
scene.add(std::make_shared<Triangle>(A,B,C,Colour(255,255,255)));
scene.add(std::make_shared<Sphere>(sph));

// Render
camera.render(tri, sph, 0, 10);
const std::string filename = "Image.ppm";
camera.save_image(filename);

return 0;
}
