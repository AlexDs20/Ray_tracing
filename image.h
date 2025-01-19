#pragma once
#include <fstream>
#include "types.h"

struct Image {
    u32 width;
    u32 height;
    u8x3* colours;
};

Image AllocateImage(u32 width, u32 height) {
    Image image = {};
    image.width = width;
    image.height = height;

    u8x3* data = (u8x3*) malloc(width*height*sizeof(u8x3));

    image.colours = data;
    return image;
}

inline f32x3 gamma_correct(f32x3 colour, f32 gamma=2.0f) {
    if (gamma == 2.0f) {
        return sqrt(colour);
    }
    return pow(colour, 1.0f/gamma);
}

void SaveImage(Image image, const char* filepath) {
    std::ofstream file;
    file.open(filepath);
    file << "P3\n# test.ppm\n" << image.width << " " << image.height << "\n255\n";

    for (u32 h=0; h<image.height; ++h) {
        for (u32 w=0; w<image.width; ++w) {
            file << " " << +image.colours[w + h * image.width].r;               // Need + to write out as number (need unary operator+() to be defined
            file << " " << +image.colours[w + h * image.width].g;
            file << " " << +image.colours[w + h * image.width].b;
        }
        file << "\n";
    }
    file.close();
};
