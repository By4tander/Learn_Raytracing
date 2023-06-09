//
// Created by 贾奕人 on 2023/5/6.
//

//------------------------
//在这个章节中我们优化了shadow acne和gamma矫正的问题
//------------------------

//shadow acne指的是在光线与表面相交时，由于浮点数精度限制或计算误差
// 导致的阴影表面上出现的小块噪点或表面不光滑的现象。

//04的图片非常暗。但是我们的球体每次反射只吸收能量的一半，所以它们是50%的反射体。如果你看不到阴影，不要担心，我们将立即修复。这些球体应该看起来相当亮（在现实生活中，是浅灰色）。原因是几乎所有的图像查看器都假设图像经过了"伽马校正"，也就是说0到1之间的值在存储为字节之前经过了某种变换。这样做有很多好的理由，但是对于我们的目的，我们只需要知道这一点即可。作为初步近似，我们可以使用"伽马2"，也就是将颜色提升到1/𝑔𝑎𝑚𝑚𝑎的幂，或者在我们的简单情况下，提升到1/2的幂，也就是平方根。

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

    if (depth <= 0)
        return color(0,0,0);

    //一些反射光线在与其反射物体碰撞时，并不完全在𝑡=0的位置上
    // 而是在𝑡=−0.0000001或𝑡=0.00000001等接近于零的浮点数近似值上。
    // 因此，我们需要忽略非常接近零的碰撞点。

    /*****************
     * ⚠️这个地方如果下届取得太大（如这里取0.1）
     * 在场景中物体变多时会出现黑影（见Results/07Matel with flaw)
     * 这是由于一些本来太小的浮点数会被编译器算作0，则按照发生碰撞计算，实际上没有
     * 但如果下界取得太大，则忽略了太多可以计算的，没有发生碰撞的近距离的点
     * 这会导致那片区域一坨黑
     */
    //if (world.hit(r, 0, infinity, rec)) {
    if (world.hit(r, 0.1, infinity, rec)) {
        point3 target = rec.p + rec.normal + random_in_unit_sphere();
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
    std::ofstream  ofs("05shadow acne.ppm",std::ios::binary);
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