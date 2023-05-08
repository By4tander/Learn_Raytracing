//
// Created by Eric on 2023/5/8.
//

#ifndef RAYTRACING_FINAL_CAMERA_H
#define RAYTRACING_FINAL_CAMERA_H

/***********We achieve to approximate a real camera******
 *For our code we could simulate the order: sensor, then lens, then aperture.
 * Then we could figure out where to send the rays,
 * and flip the image after it's computed (the image is projected upside down on the film)
 *Here we use a thin lens approximation
 *
 ************More details in Notion************/
#include "../Final_Includes/constant.h"
#include "../Final_Includes/ray.h"
class camera{
public:
    camera( point3 lookfrom,
            point3 lookat,
            vec3 vup,
            double vfov, // vertical field-of-view in degrees
            double aspect_ratio,// width/height

            //new variables
            double aperture,//光圈
            double focus_dist//相机焦距
    ){

        auto theta = degrees_to_radians(vfov);
        auto h=tan(theta/2);
        auto viewport_height=2*h;
        auto viewport_width = aspect_ratio* viewport_height;

        auto focal_length = 1.0;

        //此时uvw是私有变量，在生成光线的函数中要用到
        w= unit_vector(lookfrom-lookat);
        u= unit_vector(cross(vup,w));
        v=cross(w, u);

        origin = lookfrom;

        //注意这里现在水平和垂直向要乘以焦距
        horizontal =focus_dist* viewport_width*u;
        vertical = focus_dist*viewport_height*v;
        //注意这里左下角的计算，最后一个分量减去的是w
        lower_left_corner = origin - horizontal/2 - vertical/2 -focus_dist* w;

        lens_radius=aperture/2;
    }

    /*****
     *  generate random scene rays originating from inside a disk centered at the lookfrom point.
     *  The larger the radius, the greater the defocus blur.
     *  You can think of our original camera as having a defocus disk of radius zero (no blur at all),
     *  so all rays originated at the disk center (lookfrom).
     *
     */
    ray get_ray(double s, double t) const{
        vec3 rd = lens_radius * random_in_unit_disk();//random 在-1，1之间，我们把范围放在透镜半径中
        vec3 offset = u * rd.x() + v * rd.y();      //偏移量

        return ray(
                origin + offset,//此时光线不是从一个点射出，而是透镜小孔
                lower_left_corner + s*horizontal + t*vertical - origin - offset
        );

    }

private:
    point3 origin;
    point3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;
    vec3 u,v,w;
    double lens_radius;//模拟透镜半径
};




#endif //RAYTRACING_FINAL_CAMERA_H
