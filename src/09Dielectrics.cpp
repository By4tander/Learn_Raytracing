//
// Created by 贾奕人 on 2023/5/8.
//

/*Check out on notion
 * The refraction is described by Snell’s law:
 * ************   𝜂⋅sin𝜃=𝜂′⋅sin𝜃′   *************
 *
 * Then we achieved Total Internal Reflection in material.h
 *
 * Eventually, we use Schlick Approximation to make it just like glass
 * For real glass has reflectivity that varies with angle
 * — look at a window at a steep angle and it becomes a mirror.
 *
 * Also, we use negative radius to create a hollow glass sphere
 */



#include "../Final_Includes/render_color.h"
#include "../Final_Includes/ray.h"
#include "../Final_Includes/hittable_list.h"
#include "../Final_Includes/Sphere_with_material.h"
#include "../common/camera.h"
#include "../Final_Includes/material.h"

#include <fstream>
#include <iostream>


color ray_color(const ray& r, const hittable& world, int depth) {
    hit_record rec;

    if (depth <= 0)
        return color(0,0,0);

    /*we use mat_ptr to get color*/
    if (world.hit(r, 0.001, infinity, rec)) {
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
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 100;
    const int max_depth = 50;

    /************** World********************/
    hittable_list world;

    auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));

    //09,change material into glass
    auto material_center = make_shared<dielectric>(1.4);
    auto material_left   = make_shared<dielectric>(1.5);

    auto material_right  = make_shared<metal>(color(0.8, 0.6, 0.2), 0.5);

    world.add(make_shared<sphere_with_mat>(point3( 0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(make_shared<sphere_with_mat>(point3( 0.0,    0.0, -1.0),   0.5, material_center));
    world.add(make_shared<sphere_with_mat>(point3( 1.0,    0.0, -1.0),   0.5, material_right));

    /*An interesting and easy trick with dielectric spheres
     * is to note that if you use a negative radius,
     * the geometry is unaffected, but the surface normal points inward.
     */
    //This can be used as a bubble to make a hollow glass sphere:
    world.add(make_shared<sphere_with_mat>(point3(-1.0,    0.0, -1.0),  -0.5, material_left));


    // Camera
    camera cam;

    // Render
    std::ofstream  ofs("09Dielectric_hollow_sphere.ppm",std::ios::binary);
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