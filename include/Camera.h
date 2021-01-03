#pragma once
#include <vector>

#include "Vector3.h"
#include "Triplet.h"
#include "Image.h"

class Screen{
  public:
    const double pixel_size;
    const unsigned int w, h;
    Triplet y, z;   // Basis direction for the screen

    Screen() : pixel_size(1), w(1024), h(768), y(Triplet(0,1,0)), z(Triplet(0,0,1)) {}
    Screen(double pix_, unsigned int w_, unsigned int h_, Triplet dir_) : pixel_size(pix_), w(w_), h(h_) {
      Triplet dir = dir_/dir_.norm();
      if (dir == Triplet(0,0,1)){
        z = Triplet(1,0,0);
        y = Triplet(0,1,0);
      } else {
        z = Triplet(0,0,1) - Triplet(0,0,1).dot(dir) * dir;
        z /= z.norm();
        y = z.cross(dir);
        y /= y.norm();
      }
    }
    const Triplet pixel_pos(unsigned int i, unsigned int j) const {
      // Position relative to the center of the screen!
      Triplet row = ( (h-(h+1)%2) /2. - j) *pixel_size*z;
      Triplet col = ( (w-(w+1)%2) /2. - i) *pixel_size*y;
      return row+col;
    }
};

class Camera{
  public:
    Vector3 dir;
    double dist_screen;
    Screen screen;
    Image img;

   Camera() : dir(Vector3(1,0,0)), dist_screen(1), screen(0.1, 1024, 768, Triplet(1,0,0)), img(1024,768) {}

   Camera(Vector3 pos_dir_camera_, double dist_, unsigned int w_, unsigned int h_ , double pix_size_, Colour bg_) : \
     dir(pos_dir_camera_), dist_screen(dist_), screen(pix_size_, w_, h_, dir), img(w_, h_, bg_) {}

//    void Render(std::vector<Geometry> objs_){
//      Vector3 ray_pix0(dir.start, dir(dist_screen) + screen.pixel_pos(0,0));
//
//    }

};

