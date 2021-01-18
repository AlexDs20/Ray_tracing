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
Triplet cam_pos(-10.0, 0.0, 2.0);
Triplet cam_look(0,0,0.5);
Vector3 cam_vec(cam_pos, cam_look);

Camera camera(cam_vec, 0.1, 100*PI/180, 2*1024, 16/9.);

// Create the materials
auto material_ground = std::make_shared<Lambertian>(Colour(0.8, 0.8, 0.0));
auto material_front  = std::make_shared<Dielectric>(Colour(1.0, 1.0, 1.0), 1.7);
auto material_left   = std::make_shared<Metal>(Colour(0.8, 0.8, 0.8), 0.0);
auto material_center = std::make_shared<Lambertian>(Colour(0.7, 0.3, 0.0));
auto material_right  = std::make_shared<Lambertian>(Colour(0.8, 0.6, 0.2));

// Create the objects
Hittable_list scene;
Triplet A1(-20.0,  0.0, -0.5);
Triplet A2( 5.5, -50.0, -0.5);
Triplet A3( 5.5,  50.0, -0.5);

scene.add(std::make_shared<Triangle>(A1, A2, A3, material_ground));

scene.add(std::make_shared<Sphere>(Triplet(-2.0,   0.0,   0.0),   0.5, material_front));
scene.add(std::make_shared<Sphere>(Triplet( 0.0,   1.0,   1.0),   0.5, material_left));
scene.add(std::make_shared<Sphere>(Triplet( 0.0,   0.0,   1.0),   0.5, material_center));
scene.add(std::make_shared<Sphere>(Triplet( 0.0,  -1.0,   1.0),   0.5, material_right));

// Pyramid
if (1){
  auto mirror   = std::make_shared<Metal>(Colour(0.2, 0.8, 0.4), 0.0);
  auto mirror_fuzzy   = std::make_shared<Metal>(Colour(0.2, 0.8, 0.4), 0.3);
  // basis
  Triplet B1(-1.5,  0.0, -0.5);
  Triplet B2( 0.5, -2.0, -0.5);
  Triplet B3( 0.5,  2.0, -0.5);

  // top
  Triplet T( 0.0, 0.0, 0.5);

  scene.add(std::make_shared<Triangle>(B1, B3, B2, mirror));
  scene.add(std::make_shared<Triangle>(B1, B2, T, mirror_fuzzy));
  scene.add(std::make_shared<Triangle>(B2, B3, T, mirror));
  scene.add(std::make_shared<Triangle>(B3, B1, T, mirror));
}
// Render
const unsigned int max_depth=50;
const unsigned int ray_per_pixel=100;

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
