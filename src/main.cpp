#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <cstdlib>
#include <thread>

#include "Define.h"
#include "Vector3.h"
#include "Triplet.h"
#include "Colour.h"
#include "Image.h"
#include "Hittable.h"
#include "Hittable_list.h"
#include "Camera.h"
#include "Utilities.h"
#include "Material.h"

int main(){

// Set the random seed
srand( (unsigned)time(NULL) );

// Create the camera
Triplet cam_pos(-1.5,0,1.5);
Triplet cam_look(0,0,0);
Vector3 cam_vec(cam_pos, cam_look);
int scale = 1;
Camera camera(cam_vec, 1., 1024/scale, 768/scale, scale*0.01, Colour(0,0,0));

// Create the materials
auto material_ground = std::make_shared<Lambertian>(Colour(0.8, 0.8, 0.0));
auto material_center = std::make_shared<Lambertian>(Colour(0.7, 0.3, 0.3));
auto material_left   = std::make_shared<Metal>(Colour(0.8, 0.8, 0.8));
auto material_right  = std::make_shared<Metal>(Colour(0.8, 0.6, 0.2));

// Create the objects
Hittable_list scene;
Triplet A(-20.0,  0.0, -0.5);
Triplet B( 2.0, -50.0, -0.5);
Triplet C( 2.0,  50.0, -0.5);

scene.add(std::make_shared<Triangle>(A, B, C, material_ground));
scene.add(std::make_shared<Sphere>(Triplet( 0.0,    2.0,    0.0),   1.0, material_left));
scene.add(std::make_shared<Sphere>(Triplet( 0.0,    0.0,    0.0),   1.0, material_center));
scene.add(std::make_shared<Sphere>(Triplet( 0.0,   -2.0,    0.0),   1.0, material_right));


// Render
const unsigned int max_depth=50;
const unsigned int ray_per_pixel=20;

// Multi-threading
int n_procs = (int)std::thread::hardware_concurrency();
//n_procs = 1;
if (n_procs==1)
  camera.render(scene, ray_per_pixel, max_depth, 1);
else{
  std::vector<std::thread> procs;

  for(int i=0; i<n_procs; i++)
    procs.push_back(std::thread(&Camera::render, &camera, scene, ray_per_pixel, max_depth, n_procs));

  for(int i=0; i<n_procs; i++)
    procs[i].join();
}

// Save Image
const std::string filename = "Image.ppm";
camera.save_image(filename);

return 0;
}
