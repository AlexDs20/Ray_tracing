#include <stdio.h>
#include <cstdlib>

#include <iostream>
#include <fstream>

#include "types.h"

struct Camera {
    u32 width;
    u32 height;
    f32 viewport_height = 9.0f/16.0f;
    f32 viewport_width = 1.0f;
    f32 aspect_ratio = 16.0f/9.0f;

    f32x3 O = {0.0f, 0.0f, 1.0f};
    f32x3 dir = {0.0f, 0.0f, -1.0f};
    f32x3 up = {0.0f, 1.0f, 0.0f};
    f32 focal_length = 1.0f;
};

struct Ray {
    f32x3 O;
    f32x3 dir;
};

struct Sphere {
    f32x3 pos;
    f32 r;
};

void write_ppm_from_array(const f32x4* data, const u32 height, const u32 width, const char* filepath) {
    std::ofstream file;
    file.open(filepath);
    file << "P3\n# test.ppm\n" << width << " " << height << "\n255\n";

    for (u32 h=0; h<height; ++h) {
        for (u32 w=0; w<width; ++w) {
            file << " " << (u32)(255*data[w + h * width].x);
            file << " " << (u32)(255*data[w + h * width].y);
            file << " " << (u32)(255*data[w + h * width].z);
        }
        file << "\n";
    }
    file.close();
    printf("File written: %s\n", filepath);
}

bool ray_sphere_intersect(Ray& ray, Sphere& sphere){
    f32x3 C = sphere.pos;
    f32x3 Q = ray.O;
    f32x3 QC = Q-C;
    f32 half_b = -dot(ray.dir, QC);
    f32 a = 1.0f;       // length2(ray.dir);
    f32 c = length2(QC) - sphere.r*sphere.r;

    f32 delta = half_b*half_b - a * c;

    if (delta<0) {
        return false;
    }

    return true;
}


int main() {
    Camera camera = { 960, 540 };

    f32x4* result = (f32x4*)malloc(camera.width * camera.height * sizeof(f32x4));

    const u32 N = 2;
    Sphere spheres[N];
    spheres[0] = {
        {-0.4f, -0.1f, -2.5f},
        0.1f
    };
    spheres[1] = {
        {0.25f, 0.1f, -3.5f},
        0.2f
    };

    const f32x4 background = {0.7f, 0.3f, 0.2f, 1.0f};
    const f32x4 sphere_color = {0.2f, 0.3f, 0.6f, 1.0f};

    f32x3 w_dir = cross(camera.dir, camera.up);
    f32x3 h_dir = cross(w_dir, camera.dir);

    f32 half_width =  (f32)camera.viewport_width  * 0.5f;
    f32 half_height = (f32)camera.viewport_height * 0.5f;
    f32 res = camera.viewport_width / camera.width;
    f32x3 viewport_center = camera.O + camera.focal_length*camera.dir;

    for (u32 h=0; h<camera.height; ++h) {
        for (u32 w=0; w<camera.width; ++w) {
            // Go through pixels from top left to bottom right
            f32 w_shift = -half_width  + ((f32)w+0.5f) * res;
            f32 h_shift =  half_height - ((f32)h+0.5f) * res;
            // TODO(alex): Computation of pixel pos are probably not fully correct...
            f32x3 pixel_pos = viewport_center + w_shift * w_dir + h_shift*h_dir;

            Ray ray = { camera.O, normalize(pixel_pos - camera.O) };

            bool no_collision = true;
            for (u32 n=0; n<N; n++) {
                if (ray_sphere_intersect(ray, spheres[n])){
                    result[w + h*camera.width] = sphere_color;
                    no_collision = false;
                    break;
                }
            }

            if (no_collision) {
                result[w + h*camera.width] = background;
            }
        }
    }

    write_ppm_from_array(result, camera.height, camera.width, "test.ppm");
    free(result);
}
