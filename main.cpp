#include <stdlib.h>
#include <cstdlib>
#include <cfloat>

#include "bvh.h"
#include "types.h"
#include "image.h"
#include "intersect.h"
#include "rand.h"
#include "object.h"


struct Camera {
    f32x3 O = {5.0f, 2.0f, 13.0f};
    f32x3 lookAt = {0.0f, 0.0f, 0.0f};
    f32x3 dir = normalize(lookAt-O);
    f32x3 up = {0.0f, 1.0f, 0.0f};
    f32 focalLength = 0.7f;
    f32 viewportWidth = 1.0f;
    f32 aspectRatio = 16.0f / 9.0f;

    f32 half_width;
    f32 half_height;
    f32 res;
    f32x3 w_dir;
    f32x3 h_dir;
    f32x3 viewportCenter = O + focalLength * dir;
};

f32 reflectance(f32 cos_theta, f32 refractive_index) {
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
}

void first_scene(const char* filepath) {
    Image image = AllocateImage(1024, 576);
    AllocateImage(image.width, image.height);

    const u32 max_depth = 32;
    const u32 rays_per_pixel = 1;
    const f32 rpp_factor = 1.0f / rays_per_pixel;

    Camera camera;
    camera.half_width = camera.viewportWidth * 0.5f;;
    camera.half_height = camera.half_width / camera.aspectRatio;
    camera.res = camera.viewportWidth / image.width;
    camera.w_dir = cross(camera.dir, camera.up);
    camera.h_dir = cross(camera.w_dir, camera.dir);

    const u32 n_mat = 20;
    Material mat[n_mat] = {};
    {
        mat[0].type = Mat_type::LAMBERTIAN;
        mat[0].colour = { 1.0f,  1.0f,  1.0f };

        mat[1].type = Mat_type::LAMBERTIAN;
        mat[1].colour = { 0.3f,  0.7f,  0.3f };

        mat[2].type = Mat_type::METAL;
        mat[2].colour = { 0.7f, 0.49f, 0.19f };
        mat[2].fuzziness = 0.1f;

        mat[3].type = Mat_type::LAMBERTIAN;
        mat[3].colour = { 0.4f, 0.3f, 0.7f };
        mat[3].fuzziness = 0.1f;

        mat[4].type = Mat_type::DIELECTRIC;
        mat[4].colour = { 0.2f, 0.3f, 0.8f };
        mat[4].refractive_index = Refractive_Index::GLASS;

        mat[5].type = Mat_type::LAMBERTIAN;
        mat[5].colour = { 0.1f,  0.8f,  0.8f };

        mat[6].type = Mat_type::LAMBERTIAN;
        mat[6].colour = {};
    }

    const u32 n_spheres = 20;
    Sphere spheres[n_spheres] = {};
    if (1){
        spheres[0].O = {0.0f, -50.0f, 0.0f};
        spheres[0].r = 49.0f;
        spheres[0].material_index = 1;

        spheres[1].O = {-0.5f, 0.5f, 0.0f};
        spheres[1].r = 0.5f;
        spheres[1].material_index = 2;

        spheres[2].O = {0.0f, 1.15f, 1.2f};
        spheres[2].r = 0.5f;
        spheres[2].material_index = 4;

        spheres[3].O = {0.5f, 0.5f, 0.0f};
        spheres[3].r = 0.5f;
        spheres[3].material_index = 3;

        spheres[4].O = {-1.0f, -1.0f, -1.0f};
        spheres[4].r = 0.03f;
        spheres[4].material_index = 6;

        spheres[5].O = {2.0f, 2.0f, 2.0f};
        spheres[5].r = 0.03f;
        spheres[5].material_index = 6;
    }

    World world = {};
    world.sphere_count = n_spheres;
    world.spheres = spheres;
    world.material_count = n_mat;
    world.materials = mat;

    for (u32 h=0; h<image.height; ++h) {
        for (u32 w=0; w<image.width; ++w) {

            f32x3 pixel_colour = {0.0f, 0.0f, 0.0f};

            for (u32 ray_idx=0; ray_idx<rays_per_pixel; ray_idx++) {

                // Go through pixels from top left to bottom right
                f32 w_shift = -camera.half_width  + ((f32)w+random_in_range(0.0f, 1.0f)) * camera.res;
                f32 h_shift =  camera.half_height - ((f32)h+random_in_range(0.0f, 1.0f)) * camera.res;

                f32x3 pixel_pos = camera.viewportCenter + w_shift * camera.w_dir + h_shift*camera.h_dir;
                Ray ray = { camera.O, normalize(pixel_pos - camera.O) };

                f32x3 partial_pixel_colour = cast_ray(ray, &world, max_depth);

                pixel_colour += partial_pixel_colour;

            }
            f32x3 c = 255*gamma_correct(pixel_colour * rpp_factor);
            image.colours[w + h*image.width] = { (u8)c.r, (u8)c.g, (u8)c.b };
        }
    }

    SaveImage(image, filepath);
    free(image.colours);
}


void second_scene(const char* filepath) {
    Image image = AllocateImage(1024, 576);
    // Image image = AllocateImage(1920, 1080);
    AllocateImage(image.width, image.height);

    const u32 max_depth = 32;
    const u32 rays_per_pixel = 16;
    const f32 rpp_factor = 1.0f / rays_per_pixel;

    Camera camera;
    camera.half_width = camera.viewportWidth * 0.5f;;
    camera.half_height = camera.half_width / camera.aspectRatio;
    camera.res = camera.viewportWidth / image.width;
    camera.w_dir = cross(camera.dir, camera.up);
    camera.h_dir = cross(camera.w_dir, camera.dir);

    const u32 n_spheres = 10;
    Sphere spheres[n_spheres] = {};
    for (u32 i=0; i<n_spheres; i++) {
        spheres[i].O = {
            random_in_range(-4.0f, 4.0f),
            random_in_range(-4.0f, 4.0f),
            random_in_range(-4.0f, 4.0f),
        };
        spheres[i].r = random_in_range(0.05f, 0.5f);
    }
    if (0){
        spheres[0].O = {0.0f, 0.0f, 0.0f};
        spheres[0].r = 0.2f;

        spheres[1].O = {-1.5f, 0.5f, 0.5f};
        spheres[1].r = 0.2f;

        spheres[2].O = {0.0f, 1.15f, 1.2f};
        spheres[2].r = 0.2f;

        spheres[3].O = {0.5f, 0.5f, 0.0f};
        spheres[3].r = 0.2f;

        spheres[4].O = {-1.0f, -1.0f, -1.0f};
        spheres[4].r = 0.3f;

        spheres[5].O = {2.5f, -0.4f, 1.5f};
        spheres[5].r = 0.3f;
    }

    // AABB aabb = aabb_sphere(spheres[1]);

    const u32 n_nodes = 2 * n_spheres - 1;
    BVHNode bvhnode[n_nodes];
    create_bvh_hierarchy(bvhnode, spheres, n_spheres);

    for (u32 h=0; h<image.height; ++h) {
        for (u32 w=0; w<image.width; ++w) {

            f32x3 pixel_colour = {0.0f, 0.0f, 0.0f};

            for (u32 ray_idx=0; ray_idx<rays_per_pixel; ray_idx++) {

                // Go through pixels from top left to bottom right
                f32 w_shift = -camera.half_width  + ((f32)w+random_in_range(0.0f, 1.0f)) * camera.res;
                f32 h_shift =  camera.half_height - ((f32)h+random_in_range(0.0f, 1.0f)) * camera.res;

                f32x3 pixel_pos = camera.viewportCenter + w_shift * camera.w_dir + h_shift*camera.h_dir;
                Ray ray = { camera.O, normalize(pixel_pos - camera.O) };

                // Go through BB and Sphere
                f32 t = FLT_MAX;
                int idx_sph = -1;

                for (u32 i=0; i<n_spheres; i++) {
                    f32 tmp = ray_sphere_intersect(ray, spheres[i]);
                    if ((tmp>0.0f) && (tmp < t)){
                        t = tmp;
                        idx_sph = i;
                    }
                }

                int idx_seg = -1;

                // n_nodes = 3;
                for (u32 i=0; i<n_nodes; i++) {
                    Segment segments[12];
                    create_segments_from_aabb(segments, bvhnode[i].bbox);

                    for (u32 i=0; i<12; i++) {
                        f32 tmp = ray_segment_intersect(ray, segments[i]);
                        if ((tmp>0.0f) && (tmp < t)){
                            t = tmp;
                            idx_seg = i;
                        }
                    }
                }

                f32x3 partial_pixel_colour;
                if (idx_seg != -1) {
                    partial_pixel_colour = {1.0f, 0.0f, 0.0f};
                } else if (idx_sph != -1) {
                    partial_pixel_colour = {0.3f, 0.8f, 0.3f};
                } else {
                    partial_pixel_colour = {0.6f, 0.6f, 0.8f};
                }

                pixel_colour += partial_pixel_colour;

            }
            f32x3 c = 255*gamma_correct(pixel_colour * rpp_factor);
            image.colours[w + h*image.width] = { (u8)c.r, (u8)c.g, (u8)c.b };
        }
    }

    SaveImage(image, filepath);
    free(image.colours);
}


int main(int argc, char** argv) {
    // first_scene(argv[1]);
    second_scene(argv[1]);
}
