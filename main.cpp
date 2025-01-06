#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <cfloat>

#include <iostream>
#include <fstream>

#include "types.h"

#define EPS 1e-4f


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
        f32 refractive_index;
    };
};

struct Ray {
    f32x3 O;
    f32x3 dir;
    // f32 refrective_index;
};

struct Sphere {
    f32x3 O;
    f32 r;
    u32 material_index;
};

struct World {
    u32 sphere_count;
    Sphere* spheres;

    u32 material_count;
    Material* materials;
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

f32 ray_sphere_intersect(Ray& ray, Sphere& sphere) {
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

    if (abs(t) > EPS) {
        return t;
    }

    t = (half_b + sqrt(delta))/a;
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

f32
reflectance(f32 cos_theta, f32 refractive_index) {
    f32 r0 = (1 - refractive_index) / (1 + refractive_index);
    r0 *= r0;
    return r0 + (1-r0)*pow(1-cos_theta, 5);
}

inline f32x3
cast_ray(Ray& ray, World* world, u32 max_depth) {
    Sphere* spheres = world->spheres;
    u32 n_spheres = world->sphere_count;
    Material* materials = world->materials;

    f32x3 out_colour = { 1.0f,  1.0f,  1.0f };

    for (u32 depth=0; depth<max_depth; depth++) {

        f32 t = FLT_MAX;
        int s = -1;

        for (u32 i=0; i<n_spheres; i++) {
            f32 tmp = ray_sphere_intersect(ray, spheres[i]);
            if ((tmp>0.0f) && (tmp < t)){
                t = tmp;
                s = i;
            }
        }

        if (s == -1) {
            out_colour *= materials[0].colour;
            break;
        }
        if (depth == max_depth-1) {
            out_colour = {0.0f, 0.0f, 0.0f };
            break;
        }

        // If träff sfär and not last depth
        {
            // Get info at intersection point
            f32x3 x_intersect = ray.O + t * ray.dir;
            f32x3 N = (x_intersect - spheres[s].O)/spheres[s].r;
            const Material& mat = materials[spheres[s].material_index];
            f32x3 colour = mat.colour;
            f32x3 new_dir;

            // Compute what to do with ray depending on the surface type
            if (mat.type == Mat_type::LAMBERTIAN) {
                new_dir = normalize(N + random_vector_on_unit_sphere());

            }  else if (mat.type == Mat_type::METAL) {
                new_dir = normalize(ray.dir - 2*dot(ray.dir, N)*N);
                if (mat.fuzziness > 0.0f) {
                    f32x3 fuzz = mat.fuzziness * random_vector_in_unit_sphere();
                    new_dir = normalize( new_dir + fuzz );
                }

            } else if (mat.type == Mat_type::DIELECTRIC) {

                // TODO(alex): change to use the curren ray's environment refractive index
                f32 ri = dot(ray.dir, N) < 0 ? Refractive_Index::AIR/mat.refractive_index : mat.refractive_index;
                N = dot(ray.dir, N) > 0 ? -N : N;
                f32 cos_theta = -dot(ray.dir, N);
                f32 sin_theta = sqrt(1.0f - cos_theta*cos_theta);

                if (ri * sin_theta > 1.0f || reflectance(cos_theta, ri) > random_in_range(0.0f, 1.0f)) {
                    new_dir = normalize(ray.dir - 2*dot(ray.dir, N)*N);
                } else {
                    colour = {1.0f, 1.0f, 1.0f};
                    f32x3 out_perp = ri * (ray.dir + cos_theta * N);
                    f32x3 out_para = -sqrt(1.0f-fmin(1.0f, length2(out_perp)))*N;
                    new_dir = normalize(out_perp + out_para);

                    // new_dir = ri * ray.dir + (ri*cos_theta - sqrt(1.0f - ri*ri * (1-cos_theta*cos_theta)))*N;
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
    // Camera camera = { 624, 351 };
    Camera camera = { 1024, 576 };
    // Camera camera = { 1920, 1080 };

    const u32 max_depth = 8;
    const u32 rays_per_pixel = 32;
    const f32 rpp_factor = 1.0f / rays_per_pixel;

    f32x3* result = (f32x3*)malloc(camera.width * camera.height * sizeof(f32x3));

    Material mat[6] = {};
    {
        mat[0].type = Mat_type::LAMBERTIAN;
        mat[0].colour = { 0.3f,  0.3f,  0.7f };

        mat[1].type = Mat_type::LAMBERTIAN;
        mat[1].colour = { 0.7f,  0.3f,  0.7f };

        mat[2].type = Mat_type::METAL;
        mat[2].colour = { 0.6f, 0.2f, 0.3f };

        mat[3].type = Mat_type::METAL;
        mat[3].colour = { 0.6f, 0.2f, 0.3f };
        mat[3].fuzziness = 0.1f;

        mat[4].type = Mat_type::DIELECTRIC;
        mat[4].colour = { 0.2f, 0.3f, 0.6f };
        mat[4].refractive_index = Refractive_Index::GLASS;

        mat[5].type = Mat_type::LAMBERTIAN;
        mat[5].colour = { 0.1f,  0.3f,  0.6f };
    }

    Sphere spheres[4] = {};
    {
        spheres[0].O = {0.0f, -50.0f, 0.0f};
        spheres[0].r = 50.0f;
        spheres[0].material_index = 1;

        spheres[1].O = {-0.5f, 0.5f, 0.0f};
        spheres[1].r = 0.5f;
        spheres[1].material_index = 2;

        spheres[2].O = {0.0f, 1.15f, 1.2f};
        spheres[2].r = 0.5f;
        spheres[2].material_index = 5;

        spheres[3].O = {0.5f, 0.5f, 0.0f};
        spheres[3].r = 0.5f;
        spheres[2].material_index = 4;
    }

    World world = {};
    world.sphere_count = 4;
    world.spheres = spheres;
    world.material_count = 6;
    world.materials = mat;

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

                f32x3 pixel_pos = viewport_center + w_shift * w_dir + h_shift*h_dir;
                Ray ray = { camera.O, normalize(pixel_pos - camera.O) };

                f32x3 partial_pixel_colour = cast_ray(ray, &world, max_depth);

                pixel_colour += partial_pixel_colour;

            }
            result[w + h*camera.width] = gamma_correct(pixel_colour * rpp_factor);
        }
    }

    write_ppm_from_array(result, camera.height, camera.width, argv[1]);
    free(result);
}
