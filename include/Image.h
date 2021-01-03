#pragma once
#include <iostream>
#include <fstream>

#include "Colour.h"

class Image{
  private:
    const unsigned int w, h;
    Colour *pixels;

  public:
    Image() : w(1024), h(768) {}
    Image(const unsigned w_, const unsigned h_, const Colour &bg = Colour(0,0,0)) : w(w_), h(h_), pixels(nullptr) {
      pixels = new Colour[w*h];
      for (size_t i=0; i<w*h; ++i)
        pixels[i] = bg;
    }
    ~Image(){
      if (pixels!=NULL)
        delete [] pixels;
    }

    void set (unsigned int i, unsigned int j, const Colour& c){
      if (i<=w && j<=h)
        pixels[j*w+i] = c;
    }

    void save_PPM(const std::string &filename){
      std::fstream file(filename, std::fstream::out);
      file << "P3\n" << w << " " << h << "\n255\n";
//      file << "P6\n" << w << " " << h << "\n255\n";

      for (unsigned int i=0; i<w*h; ++i)
          file << pixels[i].r() << ' ' << pixels[i].g() << ' ' << pixels[i].b() << ' ';
//          file << (char) pixels[i].r() << (char) pixels[i].g() << (char) pixels[i].b();

      file.close();
    }
};
