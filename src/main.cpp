#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <cstdlib>

#include "Define.h"
#include "Vector3.h"
#include "Triplet.h"
#include "Colour.h"
#include "Image.h"
#include "Hittable.h"
#include "Hittable_list.h"
#include "Camera.h"
#include "Utilities.h"

int main(){

// Set the random seed
srand( (unsigned)time(NULL) );

// Create the camera
Triplet cam_pos(-1.5,0,1.5);
Triplet cam_look(0,0,0);
Vector3 cam_vec(cam_pos, cam_look);
Camera camera(cam_vec, 1., 1024, 768, 0.005, Colour(255,255,0));

// Create the objects
Triplet A(-3,  0, -1);
Triplet B(2.0,-50,-1);
Triplet C(2.0, 50,-1);
Triangle tri(A,B,C,Colour(255,255,255));

Triplet c(0,0,0);
Sphere sph(c, 1, Colour(0,255,0));

//  Create the scene of objs
Hittable_list scene;
scene.add(std::make_shared<Triangle>(A,B,C,Colour(255,255,255)));
scene.add(std::make_shared<Sphere>(sph));

// Render
const unsigned int max_depth=50;
const unsigned int ray_per_pixel= 10;
camera.render(scene, ray_per_pixel, max_depth);

// Save Image
const std::string filename = "Image.ppm";
camera.save_image(filename);

return 0;
}
