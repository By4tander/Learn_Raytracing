#include "../common/render_color.h"
#include "../common/ray.h"
#include "../common/vec3.h"
#include <fstream>


//判断是否与球体相交
bool hit_sphere(const point3& center, double radius, const ray&r)
{
    //the full form of the ray 𝐏(𝑡):(𝐀+𝑡𝐛−𝐂)⋅(𝐀+𝑡𝐛−𝐂)=𝑟2
    //expand the equation we get:
    //𝑡2𝐛⋅𝐛+2𝑡𝐛⋅(𝐀−𝐂)+(𝐀−𝐂)⋅(𝐀−𝐂)−𝑟2=0
    //相当于二次方程，自变量为t
    vec3 oc = r.origin()-center;
    auto a= dot(r.direction(),r.direction());
    auto b = 2.0 * dot(oc, r.direction());
    auto c = dot(oc, oc) - radius*radius;

    //运用二次函数判别式discriminant判断是否相交
    auto discriminant = b*b - 4*a*c;

    return (discriminant > 0);
}

color ray_color(const ray& r) {

    //如果与光线相交设置颜色为红色
    if (hit_sphere(point3(0,0,-1), 0.5, r))
        return color(1, 0, 0);

    //将光线方向单位化
    vec3 unit_direction = unit_vector(r.direction());

    auto t = 0.5*(unit_direction.y() + 1.0);

    //对背景线性插值，blendedValue=(1−𝑡)⋅startValue+𝑡⋅endValue,
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
    auto focal_length=1.0;//投影平面到投影点camera的距离，不同于focal distance

    //总是从camera发出，也就是origin：0，0，0
    auto origin=point3(0,0,0);
    auto horizontal=vec3(viewport_width,0,0);
    auto vertical =vec3(0,viewport_height,0);

    //一个从原点指向左下角的向量
    auto lower_left_corner=origin-horizontal/2-vertical/2-vec3(0,0,focal_length);


    // Render
    std::ofstream ofs("00Basicmath.ppm",std::ios::binary);
    ofs << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    //从左上到右下
    for (int j = image_height-1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {

            //u和v分别是水平和垂直的分量
            auto u = double(i) / (image_width-1);
            auto v = double(j) / (image_height-1);

            //获取ray光线，从原点出发，方向是从左下角开始
            ray r(origin, lower_left_corner + u*horizontal + v*vertical - origin);
            color pixel_color = ray_color(r);
            write_color(ofs, pixel_color);
        }
    }

    std::cerr << "\nDone.\n";
}
