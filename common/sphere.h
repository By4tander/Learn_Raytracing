//
// Created by 贾奕人 on 2023/5/3.
//

#ifndef RAYTRACING_SPHERE_H
#define RAYTRACING_SPHERE_H

#include "hittable.h"
#include "vec3.h"

//sphere类，是hittable的子类，继承自hittable
class sphere : public hittable
        {
public:
    sphere() {}
    sphere(point3 cen, double r) : center(cen), radius(r) {};

    virtual bool hit(
            const ray& r, double t_min, double t_max, hit_record& rec) const override;

public:
    //两个成员变量，球心和半径
    point3 center;
    double radius;
};

bool sphere::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    vec3 oc = r.origin() - center;
    //−𝑏±√(𝑏2−4𝑎𝑐)/2a==−ℎ±√(ℎ2−𝑎𝑐)/𝑎
    auto a = r.direction().length_squared();
    auto half_b = dot(oc, r.direction());
    auto c = oc.length_squared() - radius*radius;

    //判别式<0未相交
    auto discriminant = half_b*half_b - a*c;
    if (discriminant < 0) return false;

    auto sqrtd = sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range.
    auto root = (-half_b - sqrtd) / a;

    //因为二次函数有两个解，如果是一个物体，那么两个解分别是物体的前后边界
    //但因为现在计算的是“hittable”情况，也就是说可能存在多个物体
    //我们要计算的是可以看到的物体，也就是离得最近的那个面，
    if (root < t_min || t_max < root) {
        root = (-half_b + sqrtd) / a;
        if (root < t_min || t_max < root)
            return false;
    }

    rec.t = root;
    rec.p = r.at(rec.t);

    //计算法线，方向圆心指向外
    vec3 outward_normal=(rec.p-center)/radius;
    rec.set_face_normal(r,outward_normal);

    return true;
}
#endif //RAYTRACING_SPHERE_H
