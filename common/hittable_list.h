//
// Created by 贾奕人 on 2023/5/4.
//

//如果场景中只有一个物体，可以直接调用该物体的 hit() 函数；
// 但如果场景中有多个物体，我们需要使用 hittable_list 类来组合它们
// 并调用 hittable_list 的 hit() 函数来检测光线是否与任何一个物体相交。

#ifndef RAYTRACING_HITTABLE_LIST_H
#define RAYTRACING_HITTABLE_LIST_H

#include "hittable.h"

#include <memory>
#include <vector>

using std::shared_ptr;
using std::make_shared;

//继承hittable
class hittable_list : public hittable {
public:
    hittable_list() {}
    hittable_list(shared_ptr<hittable> object) { add(object); }

    //成员函数，清空和增加objects中的hittable对象
    void clear() { objects.clear(); }
    void add(shared_ptr<hittable> object) { objects.push_back(object); }

    //检测一条光线是否与 hittable_list
    // 中的任何一个 hittable 对象相交，并返回光线与物体相交的信息
    virtual bool hit(
            const ray& r, double t_min, double t_max, hit_record& rec) const override;

public:
    //objects 成员是一个 vector
    // 其中存储了多个指向 hittable 类型对象的 shared_ptr 对象
    std::vector<shared_ptr<hittable>> objects;
};

bool hittable_list::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    hit_record temp_rec;
    bool hit_anything = false;

    //t_max代表了光线在场景任意物体求交的最远距离
    // closest_so_far记录目前为止最近的,初始设置为最远
    auto closest_so_far = t_max;

    for (const auto& object : objects) {
        if (object->hit(r, t_min, closest_so_far, temp_rec)) {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }

    return hit_anything;
}
#endif //RAYTRACING_HITTABLE_LIST_H
