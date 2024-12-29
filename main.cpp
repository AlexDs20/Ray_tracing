#include <cstdint>
#include <stdfloat>
#include <stdio.h>
#include <cstdlib>

#include <iostream>
#include <fstream>

typedef float f32;
typedef double f64;
typedef float f32x3[3];
typedef float f32x4[4];

typedef uint32_t u32;
typedef uint32_t u32x3[3];
typedef uint8_t u8;

struct Screen {
    u32 width;
    u32 height;
};

struct Sphere {
    f32x3 x;
    f32 r;
};

struct Scene {
    Sphere* spheres;
};

void write_ppm_from_array(const f32x4* data, const Screen& screen, const char* filepath) {
    const u32 width = screen.width;
    const u32 height = screen.height;

    std::ofstream file;
    file.open(filepath);
    file << "P3\n# test.ppm\n" << width << " " << height << "\n255\n";

    for (u32 h=0; h<height; ++h) {
        for (u32 w=0; w<width; ++w) {
            file << " " << (u32)(255*data[w + h * width][0]);
            file << " " << (u32)(255*data[w + h * width][1]);
            file << " " << (u32)(255*data[w + h * width][2]);
        }
        file << "\n";
    }
    file.close();
    printf("File written: %s\n", filepath);
}

int main() {
    Screen screen = {400, 400};

    f32x4* result = (f32x4*)malloc(4 * screen.width * screen.height * sizeof(f32));

    const u32 N = 2;
    Sphere spheres[N];
    spheres[0] = {{0.25f, 0.75f, 0.3f}, 0.1f};
    spheres[1] = {{0.65f, 0.15f, 0.3f}, 0.05f};
    Scene scene = {
        .spheres=&spheres[0]
    };

    const f32x4 background = {0.7f, 0.3f, 0.2f, 1.0f};
    const f32x4 sphere_color = {0.2f, 0.3f, 0.6f, 1.0f};


    for (u32 h=0; h<screen.height; ++h) {
        for (u32 w=0; w<screen.width; ++w) {
            f32 x = (f32)w/screen.width;
            f32 y = (f32)h/screen.height;
            bool no_collision = true;
            for (u32 n=0; n<N; n++) {
                f32 x_diff = (x-scene.spheres[n].x[0]);
                f32 y_diff = (y-scene.spheres[n].x[1]);
                f32 r = scene.spheres[n].r;

                if ( x_diff*x_diff + y_diff*y_diff < r*r ) {
                    result[w + h*screen.width][0] = sphere_color[0];
                    result[w + h*screen.width][1] = sphere_color[1];
                    result[w + h*screen.width][2] = sphere_color[2];
                    result[w + h*screen.width][3] = sphere_color[3];
                    no_collision = false;
                    break;
                }
            }

            if (no_collision) {
                // RGBA
                result[w + h*screen.width][0] = background[0];
                result[w + h*screen.width][1] = background[1];
                result[w + h*screen.width][2] = background[2];
                result[w + h*screen.width][3] = background[3];
            }
        }
    }

    write_ppm_from_array(result, screen, "test.ppm");
}
