//
// Created by 贾奕人 on 2023/5/7.
//

/*07对于我们的程序，材料需要做两件事：

产生散射光（或者说它吸收了入射光）。
如果散射，说出光线应该衰减多少。*/


#ifndef RAYTRACING_MATERIAL_H
#define RAYTRACING_MATERIAL_H

#include "ray.h"

/*
 *
 *我们在这里设置的是材质将告诉我们光线如何与表面相互作用。
 * hit_record只是一种将一堆参数封装到一个结构体中的方式，
 * 以便我们可以作为一个组发送它们。当一条光线撞到表面（例如特定的球体）时，
 * hit_record中的材质指针将被设置为指向在主函数中设置球体时给定的材质指针。
 * 当ray_color()函数获取hit_record时，
 * 它可以调用材质指针的成员函数来找出是否有光线散射，以及如何散射。
 */

struct hit_record;

class material{
public:
    virtual bool scatter(
            //三个光线变量：进入的光线，衰减，分散
            const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
    ) const = 0;
};


//07Lambertian
/*
 * 它可以始终进行散射，并通过其反射率 𝑅 进行衰减，或者它可以进行散射但不衰减，
 * 而是吸收 1−𝑅 的光线的一部分，或者它可以是上述策略的混合。
 * 对于兰伯特材质，我们可以得到以下简单的类：
 */
class lambertian : public material {
public:
    lambertian(const color &a) : albedo(a) {}

    virtual bool scatter(
            const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered
    ) const

    /*
    *请注意，我们也可以只以某个概率 𝑝 进行散射，并使衰减为 𝑎𝑙𝑏𝑒𝑑𝑜/𝑝 。
    */

    override {
        //散射方向
        auto scatter_direction = rec.normal + random_unit_vector();

        /*如果你仔细阅读上面的代码，你会注意到一种可能出现问题的情况。
        * 如果我们生成的随机单位向量恰好与法线向量相反，它们将相加为零，导致散射方向向量为零。
        * 这将在后续过程中引起问题（无穷大和 NaN），因此我们需要在传递之前拦截这种情况*/
        // Catch degenerate scatter direction
        if (scatter_direction.near_zero())
            scatter_direction = rec.normal;

        scattered = ray(rec.p, scatter_direction);
        attenuation = albedo;
        return true;
    }

public:
    color albedo;
};

class metal : public material {
public:
    metal(const color& a) : albedo(a) {}

    virtual bool scatter(
            const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
    ) const override {
        vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
        scattered = ray(rec.p, reflected);
        attenuation = albedo;
        return (dot(scattered.direction(), rec.normal) > 0);
    }

public:
    color albedo;
};

#endif //RAYTRACING_MATERIAL_H
