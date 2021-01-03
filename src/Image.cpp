/*
#include <iostream>
#include <fstream>
#include "Image.h"

Image::Image(const unsigned int &w_, const unsigned int &h_, const Colour &bg_ = Colour(0,0,0)) \
        : w(w_), h(h_), pixels(nullptr) {

  pixels = new Colour[w*h];

  for (size_t i=0; i<w*h; i++)
    pixels[i] = bg_;
}

Image::~Image(){
  if (pixels!=NULL)
    delete [] pixels;
}

void Image::set(const unsigned int &i, const unsigned int &j, const Colour& c) const {
  if (i<=w && j<=h)
    this->pixels[j*w+i] = c;
}

void Image::save_PPM(const std::string &filename) const {
  std::fstream file(filename, std::fstream::out);
    file << "P3\n" << w << " " << h << "\n255\n";

  for (unsigned int i=0; i<w*h; ++i)
      file << pixels[i].r() << ' ' << pixels[i].g() << ' ' << pixels[i].b() << ' ';

  file.close();
}
*/
