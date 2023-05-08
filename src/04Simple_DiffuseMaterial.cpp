//
// Created by 贾奕人 on 2023/5/5.
//

//Now that we have objects and multiple rays per pixel,
// we can make some realistic looking materials.
//start with 哑光(matte) materials.

//first we pick a random point in a unit radius sphere
//Reject this point and try again if the point is outside the sphere.

#include "../Final_Includes/render_color.h"
#include "../Final_Includes/ray.h"
#include "../Final_Includes/vec3.h"
#include "../Final_Includes/hittable_list.h"
#include "sphere.h"
#include "../Final_Includes/constant.h"
#include "../common/camera.h"

#include <fstream>
#include <iostream>


color ray_color(const ray& r, const hittable& world, int depth) {
    hit_record rec;
    //We add diffuse part in.
    // When will it stop recursing? When it fails to hit anything.
    // If we've exceeded the ray bounce limit, no more light is gathered.
    if (depth <= 0)
        return color(0,0,0);


    if (world.hit(r, 0, infinity, rec)) {
        point3 target = rec.p + rec.normal + random_in_unit_sphere();
        return 0.8 * ray_color(ray(rec.p, target - rec.p), world, depth-1);
    }

    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5*(unit_direction.y() + 1.0);
    return (1.0-t)*color(1.0, 1.0, 1.0) + t*color(0.5, 0.7, 1.0);
}


int main() {

    // Image
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 100;

    //set the recursing depth in ray_color()
    //because In some cases,
    // ray bounces too many times that may be
    // long enough to blow the stack.
    const int max_depth = 50;

    // World
    hittable_list world;
    world.add(make_shared<sphere>(point3(0,0,-1), 0.5));
    world.add(make_shared<sphere>(point3(0,-100.5,-1), 100));

    // Camera
    camera cam;

    // Render
    std::ofstream  ofs("04dep50_gamma-correct.ppm",std::ios::binary);
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