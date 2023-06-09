//
// Created by 贾奕人 on 2023/5/8.
//


#include "render_color.h"
#include "ray.h"
#include "hittable_list.h"
#include "Sphere_with_material.h"
#include "material.h"
#include "../common/Positionable_Camera.h"

#include <fstream>
#include <iostream>


color ray_color(const ray& r, const hittable& world, int depth) {
    hit_record rec;

    if (depth <= 0)
        return color(0,0,0);

    /*we use mat_ptr to get color*/
    if (world.hit(r, 0.0001, infinity, rec)) {
        ray scattered;
        color attenuation;
        if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
            return attenuation * ray_color(scattered, world, depth-1);
        return color(0,0,0);
    }

    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5*(unit_direction.y() + 1.0);
    return (1.0-t)*color(1.0, 1.0, 1.0) + t*color(0.5, 0.7, 1.0);
}


int main() {

    // Image
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 600;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 100;
    const int max_depth = 50;

    /************** World********************/

    hittable_list world;

    auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
    auto material_center = make_shared<lambertian>(color(0.1, 0.2, 0.5));
    auto material_left   = make_shared<dielectric>(1.5);
    auto material_right  = make_shared<metal>(color(0.8, 0.6, 0.2), 0.0);

    world.add(make_shared<sphere_with_mat>(point3( 0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(make_shared<sphere_with_mat>(point3( 0.0,    0.0, -1.0),   0.5, material_center));
    world.add(make_shared<sphere_with_mat>(point3(-1.0,    0.0, -1.0),   0.5, material_left));
    world.add(make_shared<sphere_with_mat>(point3(-1.0,    0.0, -1.0), -0.45, material_left));
    world.add(make_shared<sphere_with_mat>(point3( 1.0,    0.0, -1.0),   0.5, material_right));

    /**************Camera, now we can decide more details**********/
   // camera cam(point3(-2,2,1), point3(0,0,-1), vec3(0,1,0), 90.0,aspect_ratio);
    //closer
    camera cam(point3(-2,2,-2), point3(0,0,-1), vec3(0,1,0), 40, aspect_ratio);

    // Render
    std::ofstream  ofs("10Positionable_Camera.ppm",std::ios::binary);
    ofs << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = image_height-1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {

            //对每个像素的真实值进行计算
            color pixel_color(0,0,0);
            for (int k = 0; k < samples_per_pixel; ++k) {
                auto u=(i+random_double())/(image_width-1);
                auto v=(j+random_double())/(image_height-1);
                ray r=cam.get_ray(u,v);
                pixel_color+=ray_color(r,world,max_depth);
            }

            write_color_with_samples(ofs, pixel_color,samples_per_pixel);
        }
    }

    std::cerr << "\nDone.\n";
}