//
// Created by 贾奕人 on 2023/5/3.
//


/*
 * Hittables and materials need to know each other so there is some circularity of the references.
 * In C++ you just need to alert the compiler that the pointer is to a class,
 * which the “class material” in the hittable class below does
 */
#ifndef RAYTRACING_HITTABLE_H
#define RAYTRACING_HITTABLE_H

#include "ray.h"
//#include "material.h"

class material;

//记录光线命中物体，rec变量出处
struct hit_record{
    point3 p;
    vec3 normal;//法线
    double t;//光线t分量

    shared_ptr<material> mat_ptr;//用于材质 For07

    //记录光线来自物体内还是物体外，false来自体内
    bool front_face;
    //通过判断光线来向来设置法线方向
    inline void set_face_normal(const ray& r, const vec3& outward_normal){
        //假定法线是指向外部的，ray从外部来则法线指向射线向外。
        // 如果点积<0返回true，说明法线应该指向外侧
        front_face=dot(r.direction(),outward_normal)<0;
        normal=front_face ? outward_normal: -outward_normal;
    }

};

class hittable{
    public:
        virtual bool hit(const ray&r,double t_min,double t_max,
                         hit_record& rec)const =0;
};
#endif //RAYTRACING_HITTABLE_H
