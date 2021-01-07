#pragma once
#include <string>

#include "Vector3.h"
#include "Triplet.h"
#include "Image.h"
#include "Geometries.h"

class Screen{
  private:
    Triplet pixel00;
    const double pixel_size;
    Triplet y, z;   // Basis direction for the screen

  public:
    const unsigned int w, h;

    // Constructors
    Screen();
    Screen(const double &pix_, const unsigned int &w_, const unsigned int &h_, const Triplet &dir_);

    // Methods
    const Triplet pixel_pos(const unsigned int &i, const unsigned int &j) const;
};

class Camera{
  private:
    const Vector3 dir;
    const double dist_screen;
    Screen screen;
    Image img;

  public:
    // Constructors
    Camera();
    Camera(const Vector3 &pos_dir_camera_, const double &dist_to_screen, \
          const unsigned int &w_, const unsigned int &h_ , \
          const double &pix_size_, const Colour &bg_);

    // Methods
    void render(const Triangle &obj_, const Sphere &sph_) const;
    void save_image(const std::string &filename);
};

