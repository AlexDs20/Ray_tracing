#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <cfloat>

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
    f32x3 O;
    f32 r;
};

void write_ppm_from_array(const f32x3* data, const u32 height, const u32 width, const char* filepath) {
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

f32 clamp(f32 x, f32 minimum, f32 maximum) {
    if (x < minimum) return minimum;
    if (x > maximum) return maximum;
    return x;
}

f32 ray_sphere_intersect(Ray& ray, Sphere& sphere){
    const f32x3 C = sphere.O;
    const f32x3 O = ray.O;
    const f32x3 OC = C-O;
    const f32 half_b = dot(ray.dir, OC);
    const f32 a = 1.0f;       // length2(ray.dir);
    const f32 c = length2(OC) - sphere.r*sphere.r;

    f32 delta = half_b*half_b - a * c;

    if (delta < 0) {
        return 0.0f;
    }

    f32 t = (half_b - sqrt(delta))/a;

    return t;
}

inline f32x3 gamma_correct(f32x3 colour) {
    // return colour;
    return sqrt(colour);
}

f32 random_in_range(f32 low, f32 high) {            // inclusive min and max
    return low + ((f32)rand() / RAND_MAX) * (high - low);
}

f32x3 random_vector_sphere() {
    while(true) {
        f32 x = random_in_range(-0.5f, 0.5f);
        f32 y = random_in_range(-0.5f, 0.5f);
        f32 z = random_in_range(-0.5f, 0.5f);
        if (x*x+y*y+z*z <= 1.0f) return normalize({x, y, z});
    }
}


int main() {
    Camera camera = { 960, 540 };

    f32x3* result = (f32x3*)malloc(camera.width * camera.height * sizeof(f32x3));

    const u32 N = 2;
    Sphere spheres[N];
    spheres[0] = {
        {-0.5f, -0.1f, -2.5f},
        0.5f
    };
    spheres[1] = {
        {0.5f, -0.1f,  -2.5f},
        0.5f
    };

    // const f32x3 background = {0.7f, 0.3f, 0.2f};
    const f32x3 background = {0.5f, 0.7f, 1.0f};
    const f32x3 sphere_colour = {0.2f, 0.3f, 0.6f};
    const u32 max_depth = 5;

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

            f32x3 pixel_colour = {1.0f, 1.0f, 1.0f};

            Ray ray = { camera.O, normalize(pixel_pos - camera.O) };

            if (1) {
                u32 depth;
                for (depth=0; depth<max_depth; depth++) {

                    f32 t = FLT_MAX;
                    int s = -1;
                    for (u32 n=0; n<N; n++) {
                        f32 tmp = ray_sphere_intersect(ray, spheres[n]);
                        // TODO(alex): Do not break, keep going but only keep what is closest!
                        if ((tmp>1e-7f) && (tmp < t)){
                            t = tmp;
                            s = n;
                        }
                    }
                    if (s!=-1) {
                        // Create new ray
                        f32x3 x_intersect = ray.O + t * ray.dir;
                        f32x3 N = normalize((x_intersect - spheres[s].O));        // normalize(spheres[n].O - x_intersect); If x_intersect is good enough, just divide by sphere R
                        f32x3 random = random_vector_sphere();
                        random = dot(random, N)>0 ? random : -random;
                        ray.O = x_intersect;
                        ray.dir = random;
                        pixel_colour *= sphere_colour;
                    } else {
                        pixel_colour *= background;
                        break;
                    }

                }
                result[w + h*camera.width] = gamma_correct(pixel_colour);
            }


            if (0) {
                f32x3 pixel_colour;
                f32 t = 0.0f;
                for (u32 n=0; n<N; n++) {
                    t = ray_sphere_intersect(ray, spheres[n]);

                    if (t>1e-7f){
                        f32x3 x_intersect = ray.O + t * ray.dir;
                        f32x3 N = (spheres[n].O - x_intersect)/spheres[n].r;        // normalize(spheres[n].O - x_intersect); If x_intersect is good enough, just divide by sphere R
                        pixel_colour = 0.5f * sphere_colour;
                        break;
                    }
                }

                if (t==0) {
                    pixel_colour = background;
                }
                result[w + h*camera.width] = gamma_correct(pixel_colour);
            }
        }
    }

    write_ppm_from_array(result, camera.height, camera.width, "test.ppm");
    free(result);
}
