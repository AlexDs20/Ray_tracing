#pragma once
#include <iostream>
#include <fstream>

#include "Colour.h"

class Image{
  private:
    const unsigned int w, h;
    Colour *pixels;

  public:
    // Constructors
    Image();
    Image(const unsigned &w_, const unsigned &h_);

    // Destructor
    ~Image();

    // Methods
    void set(const unsigned int &i, const unsigned int &j, const Colour &c) const;
    void save_PPM(const std::string &filename) const;
};
