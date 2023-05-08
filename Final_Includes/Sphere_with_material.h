//
// Created by 贾奕人 on 2023/5/7.
//

/*
 * 当ray_color()函数获取hit_record时，
 * 它可以调用材质指针的成员函数来找出是否有光线散射，以及如何散射。
 * 我们必须在我们的球体类中返回包含材质引用的hit_record
 */
#ifndef RAYTRACING_Sphere_WITH_MATERIAL_H
#define RAYTRACING_Sphere_WITH_MATERIAL_H


#include "hittable.h"
#include "vec3.h"

class sphere_with_mat : public hittable
{
public:
    sphere_with_mat() {}

    //添加shared_ptr
    sphere_with_mat(point3 cen, double r, shared_ptr<material> m) 
    :center(cen), radius(r), mat_ptr(m) {};

    virtual bool hit(
            const ray& r, double t_min, double t_max, hit_record& rec) const override;

public:
    //两个成员变量，球心和半径
    point3 center;
    double radius;
    shared_ptr<material> mat_ptr;
};

bool sphere_with_mat::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    vec3 oc = r.origin() - center;
    auto a = r.direction().length_squared();
    auto half_b = dot(oc, r.direction());
    auto c = oc.length_squared() - radius*radius;

    auto discriminant = half_b*half_b - a*c;
    if (discriminant < 0) return false;
    auto sqrtd = sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range.
    auto root = (-half_b - sqrtd) / a;
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

    //07材质
    rec.mat_ptr=mat_ptr;


    return true;
}


#endif
//RAYTRACING_Sphere_WITH_MATERIAL_H
