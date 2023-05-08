//
// Created by 贾奕人 on 2023/5/8.
//

#ifndef RAYTRACING_POSITIONABLE_CAMERA_H
#define RAYTRACING_POSITIONABLE_CAMERA_H

/*从结果可以看出，决定看上去画面远近的不仅仅是相机位置
*当改变vfov时，因为横纵比决定，最后vfov减小导致视口减小
 * 但因为渲染到屏幕上像素不变，看起来就近了许多
 * /
/*****************check on notion 10************/
#include "../Final_Includes/constant.h"
#include "../Final_Includes/ray.h"
class camera{
public:

    //现在我们可以自己定义一些camera的参数。如垂直视场和横纵比
    camera( point3 lookfrom,
            point3 lookat,
            vec3 vup,
            double vfov, // vertical field-of-view in degrees
            double aspect_ratio// width/height
            ){

        auto theta = degrees_to_radians(vfov);
        auto h=tan(theta/2);
        auto viewport_height=2*h;
        auto viewport_width = aspect_ratio* viewport_height;

        auto focal_length = 1.0;

        auto w= unit_vector(lookfrom-lookat);
        auto u= unit_vector(cross(vup,w));
        auto v=cross(w, u);

        origin = lookfrom;

        //注意这里获取水平垂直的方式，用的是向量投影
        horizontal = viewport_width*u;
        vertical = viewport_height*v;
        //注意这里左下角的计算，最后一个分量减去的是w
        lower_left_corner = origin - horizontal/2 - vertical/2 - w;
    }

    ray get_ray(double s, double t) const{
        return ray(origin, lower_left_corner + s*horizontal + t*vertical - origin);
    }

private:
    point3 origin;
    point3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;
};



#endif //RAYTRACING_POSITIONABLE_CAMERA_H
