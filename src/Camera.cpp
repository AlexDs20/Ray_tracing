#include "Camera.h"

// Screen Class
Screen::Screen() : pixel_size(1), y(Triplet(0,1,0)), z(Triplet(0,0,1)), w(1024), h(768) {}

Screen::Screen(const double &pix_, const unsigned int &w_, const unsigned int &h_, \
              const Triplet &dir_) : pixel_size(pix_), w(w_), h(h_) {

    Triplet dir = dir_.unit();
    if (dir == Triplet(0,0,1)){
      z = Triplet(0,1,0);
      y = Triplet(1,0,0);
    } else if (dir == Triplet(0,0,-1)) {
      z = Triplet(0,1,0);
      y = Triplet(-1,0,0);
    } else {
      z = (Triplet(0,0,1) - Triplet(0,0,1).dot(dir) * dir).unit();
      y = z.cross(dir).unit();
    }

    Triplet row = ( (h-(h+1)%2) /2. ) *pixel_size*z;
    Triplet col = ( (w-(w+1)%2) /2. ) *pixel_size*y;
    pixel00 = row+col;
}

const Triplet Screen::pixel_pos(const unsigned int &i, const unsigned int &j) const {
  return pixel00 - i * pixel_size*y - j * pixel_size*z;
}


// Camera Class
Camera::Camera() : dir(Vector3(1,0,0)), dist_screen(1), screen(0.1, 1024, 768, Triplet(1,0,0)), img(1024,768,Colour()) {}

Camera::Camera(const Vector3 &pos_dir_camera_, const double &dist_, \
              const unsigned int &w_, const unsigned int &h_ , \
              const double &pix_size_, const Colour &bg_)\
    : dir(pos_dir_camera_), dist_screen(dist_), screen(pix_size_, w_, h_, dir), img(w_, h_, bg_) {}

void Camera::render(const Triangle &obj_, const Sphere &sph_) const {
  Triplet center_screen = dir(dist_screen);
  Triplet S = dir.start;
  Triplet E;
  Vector3 ray;
  double t=0;

  for (unsigned int i=0; i<screen.w; i++){
    std::cerr << "\rProgress: " << (int)100*i/screen.w << "% " << std::flush;
    for (unsigned int j=0; j<screen.h; j++){
      E = center_screen + screen.pixel_pos(i,j);
      ray = Vector3(S,E);
        if (obj_.intersect(ray, t))
          img.set(i,j,obj_.get_colour());
        else if (sph_.intersect(ray, t))
          img.set(i,j,sph_.get_colour());
    }
  }
}

void Camera::save_image(const std::string &filename){
  img.save_PPM(filename);
}
