#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <cfloat>

#include <iostream>
#include <fstream>

#include "types.h"

#define EPS 1e-5f

struct Camera {
    u32 width;
    u32 height;
    f32 viewport_height = 9.0f/16.0f;
    f32 viewport_width = 1.0f;
    f32 aspect_ratio = 16.0f/9.0f;

    f32x3 O = {0.0f, 1.2f, 6.0f};
    f32x3 lookAt = {0.0f, 1.0f, 0.0f};
    f32x3 dir = normalize(lookAt - O);
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
    if (abs(t) < EPS) {
        t = (half_b + sqrt(delta))/a;
    }

    return t;
}

inline f32x3 gamma_correct(f32x3 colour, f32 gamma=2.0f) {
    if (gamma == 2.0f) {
        return sqrt(colour);
    }
    return pow(colour, 1.0f/gamma);
}

f32 random_in_range(f32 low, f32 high) {            // inclusive min and max
    return low + ((f32)rand() / RAND_MAX) * (high - low);
}

f32x3 random_vector_on_unit_sphere() {
    while(true) {
        f32 x = random_in_range(-1.0f, 1.0f);
        f32 y = random_in_range(-1.0f, 1.0f);
        f32 z = random_in_range(-1.0f, 1.0f);
        if (x*x+y*y+z*z <= 1.0f) return normalize({x, y, z});
    }
}

f32x3 random_vector_in_unit_sphere() {
    while(true) {
        f32 x = random_in_range(-1.0f, 1.0f);
        f32 y = random_in_range(-1.0f, 1.0f);
        f32 z = random_in_range(-1.0f, 1.0f);
        if (x*x+y*y+z*z <= 1.0f) return {x, y, z};
    }
}

namespace Refractive_Index {
    const f32 AIR = 1.000293f;
    const f32 DIAMOND = 2.4f;
    const f32 GLASS = 1.6f;
    const f32 WATER = 1.33333f;
};

enum Mat_type{
    LAMBERTIAN,
    METAL,
    DIELECTRIC
};

struct Material {
    Mat_type type = Mat_type::LAMBERTIAN;
    f32x3 colour = {1.0f, 1.0f, 1.0f};
    union {
        f32 fuzziness;
        f32 refractive_index = Refractive_Index::AIR;
    };
};

f32
reflectance(f32 cos_theta, f32 refractive_index) {
    f32 r0 = (1 - refractive_index) / (1 + refractive_index);
    r0 *= r0;
    return r0 + (1-r0)*pow(1-cos_theta, 5);
}

inline f32x3
cast_ray(Ray ray, Sphere* spheres, Material* mat, u32 n_spheres, u32 max_depth) {
    f32x3 out_colour = { 1.0f,  1.0f,  1.0f };

    for (u32 depth=0; depth<max_depth; depth++) {

        f32 t = FLT_MAX;

        int s = -1;
        for (u32 n=0; n<n_spheres; n++) {
            f32 tmp = ray_sphere_intersect(ray, spheres[n]);
            if ((tmp>0.0f) && (tmp < t)){
                t = tmp;
                s = n;
            }
        }

        if (s == -1) {
            const f32x3 background = {1.0f, 1.0f, 1.0f};
            out_colour *= background;
            break;
        }
        if (depth == max_depth-1) {
            out_colour = {0.0f, 0.0f, 0.0f };
            break;
        }

        {
            // Get info at intersection point
            f32x3 x_intersect = ray.O + t * ray.dir;
            f32x3 N = normalize((x_intersect - spheres[s].O));        // normalize(spheres[n].O - x_intersect); If x_intersect is good enough, just divide by sphere R
            f32x3 colour = mat[s].colour;
            f32x3 new_dir;

            // Compute what to do with ray depending on the surface type
            if (mat[s].type == Mat_type::LAMBERTIAN) {
                new_dir = normalize(N + random_vector_on_unit_sphere());

            }  else if (mat[s].type == Mat_type::METAL) {
                new_dir = normalize(ray.dir - 2*dot(ray.dir, N)*N);
                if (mat[s].fuzziness > 0.0f) {
                    f32x3 fuzz = mat[s].fuzziness * random_vector_in_unit_sphere();
                    new_dir = normalize( new_dir + fuzz );
                }

            } else if (mat[s].type == Mat_type::DIELECTRIC) {

                // TODO(alex): change to use the curren ray's environment refractive index
                f32 ri = dot(ray.dir, N) < 0 ? Refractive_Index::AIR/mat[s].refractive_index : mat[s].refractive_index;
                N = dot(ray.dir, N) > 0 ? -N : N;
                f32 cos_theta = -dot(ray.dir, N);
                f32 sin_theta = sqrt(1.0f - cos_theta*cos_theta);

                if (ri * sin_theta > 1.0f || reflectance(cos_theta, ri) > random_in_range(0.0f, 1.0f)) {
                    new_dir = normalize(ray.dir - 2*dot(ray.dir, N)*N);
                } else {
                    colour = {1.0f, 1.0f, 1.0f};
                    f32x3 out_perp = ri * (ray.dir + cos_theta * N);
                    f32x3 out_para = -sqrt(1.0f-length2(out_perp))*N;
                    new_dir = out_perp + out_para;
                }

            }

            ray.O = x_intersect;
            ray.dir = new_dir;
            out_colour *= colour;
        }
    }
    return out_colour;
};


int main(int argc, char** argv) {
    // Camera camera = { 960, 540 };
    Camera camera = { 1920, 1080 };

    const f32x3 background = {1.0f, 1.0f, 1.0f};
    const u32 max_depth = 10;
    const u32 rays_per_pixel = 8;
    const f32 rpp_factor = 1.0f / rays_per_pixel;

    f32x3* result = (f32x3*)malloc(camera.width * camera.height * sizeof(f32x3));

    const u32 N = 4;
    Sphere spheres[N];
    spheres[0] = {
        {-0.5f, 0.5f, 0.0f},
        0.5f
    };
    spheres[1] = {
        {0.5f, 0.5f, 0.0f},
        0.5f
    };
    spheres[2] = {
        {0.0f, 1.15f, 1.2f},
        0.5f
    };
    spheres[3] = {
        {0.0f, -50.0f, 0.0f},
        50.0f
    };

    Material mat[N];
    mat[0] = {
        .type = Mat_type::METAL,
        .colour = { 0.2f, 0.3f, 0.6f },
        .fuzziness = 0.0f,
    };
    mat[1] = {
        .type = Mat_type::LAMBERTIAN,
        .colour = { 0.6f, 0.2f, 0.3f },
    };
    mat[2] = {
        .type = Mat_type::DIELECTRIC,
        .colour = { 1.0f,  1.0f,  0.0 },
        .refractive_index = Refractive_Index::GLASS,
    };
    mat[3] = {
        .type = Mat_type::LAMBERTIAN,
        .colour = { 0.1f, 0.7f, 0.1f },
    };

    f32x3 w_dir = cross(camera.dir, camera.up);
    f32x3 h_dir = cross(w_dir, camera.dir);

    f32 half_width =  (f32)camera.viewport_width  * 0.5f;
    f32 half_height = (f32)camera.viewport_height * 0.5f;
    f32 res = camera.viewport_width / camera.width;
    f32x3 viewport_center = camera.O + camera.focal_length*camera.dir;

    for (u32 h=0; h<camera.height; ++h) {
        for (u32 w=0; w<camera.width; ++w) {

            f32x3 pixel_colour = {0.0f, 0.0f, 0.0f};

            for (u32 ray_idx=0; ray_idx<rays_per_pixel; ray_idx++) {

                // Go through pixels from top left to bottom right
                f32 w_shift = -half_width  + ((f32)w+random_in_range(0.0f, 1.0f)) * res;
                f32 h_shift =  half_height - ((f32)h+random_in_range(0.0f, 1.0f)) * res;

                // TODO(alex): Computation of pixel pos are probably not fully correct...
                f32x3 pixel_pos = viewport_center + w_shift * w_dir + h_shift*h_dir;

                Ray ray = { camera.O, normalize(pixel_pos - camera.O) };

                f32x3 partial_pixel_colour = cast_ray(ray, spheres, mat, N, max_depth);

                pixel_colour += partial_pixel_colour;

            }
            result[w + h*camera.width] = gamma_correct(pixel_colour * rpp_factor);
        }
    }

    write_ppm_from_array(result, camera.height, camera.width, argv[1]);
    free(result);
}
