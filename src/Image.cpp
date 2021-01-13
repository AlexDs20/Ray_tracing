#include "Image.h"

Image::Image() : w(1024), h(768) {}

Image::Image(const unsigned &w_, const unsigned &h_, const Colour &bg_ = Colour(0,0,0)) : w(w_), h(h_), pixels(nullptr) {
  pixels = new Colour[w*h];
  for (size_t i=0; i<w*h; ++i)
    pixels[i] = bg_;
}

Image::~Image(){
  if (pixels!=NULL)
    delete [] pixels;
}

void Image::set(const unsigned int &i, const unsigned int &j, const Colour &c) const {
  if (i<w && j<h)
    pixels[j*w+i] += c;
}

void Image::save_PPM(const std::string &filename) const {
  std::fstream file(filename, std::fstream::out);
  file << "P3\n" << w << " " << h << "\n255\n";
//    file << "P6\n" << w << " " << h << "\n255\n";

  for (unsigned int i=0; i<w*h; ++i)
      file << (int)(255*pixels[i].r()) << ' ' << (int)(255*pixels[i].g()) << ' ' << (int)(255* pixels[i].b()) << ' ';
//        file << (char) pixels[i].r() << (char) pixels[i].g() << (char) pixels[i].b();

  file.close();
}
