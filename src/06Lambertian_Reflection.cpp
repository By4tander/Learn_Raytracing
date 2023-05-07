//
// Created by 贾奕人 on 2023/5/7.
//

/******************************************************
 我们对Lambertian分布感兴趣，它具有 cos(𝜙) 的分布。
 真正的兰伯特分布在接近法线的地方具有更高的散射光线概率，但分布更加均匀。
 这可以通过在单位球的表面上选择随机点，并沿着表面法线进行偏移来实现。
 在单位球上选择随机点可以通过在单位球内选择随机点，然后进行归一化来实现。
 ******************更改在vec3.h文件和ray_color函数内*********************/

/********************About Lambertian Reflection********
 *兰伯特反射，是指光线与具有粗糙或无光泽表面的相互作用方式
 * 在兰伯特反射中，照射在表面上的光线以非常均匀的方式在各个方向上散射。
 * 与镜面反射不同，镜面反射遵循反射定律，产生镜像般的反射效果，
 * 而兰伯特反射发生在光线与表面具有不同朝向的微观面之间相互作用的情况下。
 * 这些微观面导致入射光在多个方向上反射，产生漫反射外观。
 *
 * 兰伯特反射的关键特征是反射光的强度不依赖于观察角度。
 * 换句话说，反射光的亮度保持不变，无论观察者的位置或从哪个方向观察表面。
 * 这个特性由兰伯特余弦定律描述，该定律指出反射光的强度与表面法线和观察方向之间的角度的余弦成正比。
 * 兰伯特反射通常在非光泽表面，如纸张、织物或哑光漆等中观察到。
 * 在计算机图形学和渲染算法中，它被广泛用于模拟真实的光照和阴影效果。
 */


 /* you should be able to notice two important visual differences:

 The shadows are less pronounced after the change
 Both spheres are lighter in appearance after the change

 Both of these changes are due to the more uniform scattering of the light rays,
 fewer rays are scattering toward the normal.
 This means that for diffuse objects, they will appear lighter because more light bounces toward the camera.
 For the shadows, less light bounces straight-up,
 so the parts of the larger sphere directly underneath the smaller sphere are brighter.
 */
#include "render_color.h"
#include "ray.h"
#include "../common/vec3.h"
#include "hittable_list.h"
#include "sphere.h"
#include "../common/constant.h"
#include "../common/camera.h"

#include <fstream>
#include <iostream>


color ray_color(const ray& r, const hittable& world, int depth) {
    hit_record rec;

    if (depth <= 0)
        return color(0,0,0);

    //一些反射光线在与其反射物体碰撞时，并不完全在𝑡=0的位置上
    // 而是在𝑡=−0.0000001或𝑡=0.00000001等接近于零的浮点数近似值上。
    // 因此，我们需要忽略非常接近零的碰撞点。
    //if (world.hit(r, 0, infinity, rec)) {
    if (world.hit(r, 0.1, infinity, rec)) {
        point3 target = rec.p + rec.normal + random_unit_vector();
        return 0.6 * ray_color(ray(rec.p, target - rec.p), world, depth-1);
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

    // World
    hittable_list world;
    world.add(make_shared<sphere>(point3(0,0,-1), 0.5));
    world.add(make_shared<sphere>(point3(0,-100.5,-1), 100));

    // Camera
    camera cam;

    // Render
    std::ofstream  ofs("06Labertian Reflection.ppm",std::ios::binary);
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