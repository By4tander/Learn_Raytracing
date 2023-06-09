#include "../Final_Includes/render_color.h"
#include "../Final_Includes/ray.h"
#include "../Final_Includes/vec3.h"

#include <fstream>




//注意现在是double类型，返回t的值
double hit_sphere(const point3& center, double radius, const ray&r)
{
    //the full form of the ray 𝐏(𝑡):(𝐀+𝑡𝐛−𝐂)⋅(𝐀+𝑡𝐛−𝐂)=𝑟2
    //expand the equation we get:
    //𝑡2𝐛⋅𝐛+2𝑡𝐛⋅(𝐀−𝐂)+(𝐀−𝐂)⋅(𝐀−𝐂)−𝑟2=0

    //优化：𝑏=2ℎ
    //−𝑏±√(𝑏2−4𝑎𝑐)/2a==−ℎ±√(ℎ2−𝑎𝑐)/𝑎
    vec3 oc = r.origin()-center;
    auto a= dot(r.direction(),r.direction());
    auto b = 2.0 * dot(oc, r.direction());
    auto c = dot(oc, oc) - radius*radius;

    //运用二次函数判别式discriminant判断是否相交
    auto discriminant = b*b - 4*a*c;

    if(discriminant<0){
        return -1.0;
    }
    else {
        return (-b - sqrt(discriminant) ) / (2.0*a);
    }

}

color ray_color(const ray& r) {

    //如果与光线相交设置颜色
    auto t = hit_sphere(point3(0,0,-1), 0.5, r);
    if (t > 0.0) {
        //N为法线
        vec3 N = unit_vector(r.at(t) - vec3(0,0,-1));
        return 0.5*color(N.x()+1, N.y()+1, N.z()+1);
    }

    //将光线方向单位化
    vec3 unit_direction = unit_vector(r.direction());

    t = 0.5*(unit_direction.y() + 1.0);

    //线性插值
    return (1.0-t)*color(1.0, 1.0, 1.0) + t*color(0.5, 0.7, 1.0);
}

int main() {

    // Image
    const auto aspect_ratio=16.0/9.0;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width/aspect_ratio);

    //Camera
    auto viewport_height=2.0;
    auto viewport_width=aspect_ratio*viewport_height;
    auto focal_length=1.0;

    auto origin=point3(0,0,0);
    auto horizontal=vec3(viewport_width,0,0);
    auto vertical =vec3(0,viewport_height,0);
    auto lower_left_corner=origin-horizontal/2-vertical/2-vec3(0,0,focal_length);


    // Render
    std::ofstream  ofs("01Normals.ppm",std::ios::binary);
    ofs << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = image_height-1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            auto u = double(i) / (image_width-1);
            auto v = double(j) / (image_height-1);
            ray r(origin, lower_left_corner + u*horizontal + v*vertical - origin);
            color pixel_color = ray_color(r);
            write_color_simple(ofs, pixel_color);
        }
    }

    std::cerr << "\nDone.\n";
}
