//
// Created by 贾奕人 on 2023/5/2.
//

//vec3 class
#ifndef RAYTRACING_VEC3_H
#define RAYTRACING_VEC3_H

#include "constant.h"

#include <cmath>
#include <iostream>

using std::sqrt;

class vec3 {
public:
    //如下vec3是一种默认构造函数，无任何参数，用于对象被创建时对成员变量进行初始化。
    //冒号后对部分称为初始化列表
    vec3() : e{0,0,0} {}//类vec3的默认构造函数，e是一个数组成员变量初始化为000
    vec3(double e0, double e1, double e2) : e{e0, e1, e2} {}

    double x() const { return e[0]; }
    double y() const { return e[1]; }
    double z() const { return e[2]; }


    //负号运算符（operator-()）：表示取负操作，对vec3对象的每个元素都取相反数
    vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }

    //下标运算符（operator[]）：用于访问vec3对象的元素。
    double operator[](int i) const { return e[i]; }

    //函数返回值类型为double&，表示返回一个引用，使得外部代码可以修改vec3对象的某个元素。
    double& operator[](int i) { return e[i]; }

    //以上代码实现功能
    //vec3 v1(1, 2, 3);
    //vec3 v2 = -v1;       // 取负操作
    //double x = v1[0];    // 访问元素
    //v1[1] = 4;           // 修改元素

    //该语法中，&表示引用（reference），vec3表示向量类的名称
    // operator+=表示重载了+=运算符，const vec3 &v表示参数v是一个常量引用
    // 用于接收加法操作中的另一个向量。
    // 该函数实现了将当前向量（即this）与参数v进行相加
    // 并将结果存储在当前向量中，并返回this的引用。
    vec3& operator+=(const vec3 &v) {
        e[0] += v.e[0];
        e[1] += v.e[1];
        e[2] += v.e[2];
        return *this;
    }

    vec3& operator*=(const double t) {
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;
        return *this;
    }

    vec3& operator/=(const double t) {
        return *this *= 1/t;
    }

    double length() const {
        return sqrt(length_squared());
    }

    //获得向量长度
    double length_squared() const {
        return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
    }

    //functions below are using to generate random number
    //they will be used in "diffuse materials"
    inline static vec3 random(){
        return vec3(random_double(), random_double(), random_double());
    }

    inline static vec3 random(double min, double max) {
        return vec3(random_double(min,max), random_double(min,max), random_double(min,max));
    }


    //07用于避免等于0的情况引发的问题
    bool near_zero() const {
        // Return true if the vector is close to zero in all dimensions.
        const auto s = 1e-8;
        return (fabs(e[0]) < s) && (fabs(e[1]) < s) && (fabs(e[2]) < s);}


public:
    double e[3];
};


// Type aliases for vec3，实际上都是vec3，增加可读性
using point3 = vec3;   // 3D point,表示点坐标
using color = vec3;    // RGB color，表示颜色


// vec3 Utility Functions

//这段代码是 C++ 中对输出流对象 << 操作符的重载，
// 用于将一个 vec3 类型的对象输出到一个 std::ostream 类型的输出流对象中
inline std::ostream& operator<<(std::ostream &out, const vec3 &v) {
    return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

inline vec3 operator+(const vec3 &u, const vec3 &v) {
    return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

inline vec3 operator-(const vec3 &u, const vec3 &v) {
    return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

inline vec3 operator*(const vec3 &u, const vec3 &v) {
    return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

inline vec3 operator*(double t, const vec3 &v) {
    return vec3(t*v.e[0], t*v.e[1], t*v.e[2]);
}

inline vec3 operator*(const vec3 &v, double t) {
    return t * v;
}

inline vec3 operator/(vec3 v, double t) {
    return (1/t) * v;
}

inline double dot(const vec3 &u, const vec3 &v) {
    return u.e[0] * v.e[0]
           + u.e[1] * v.e[1]
           + u.e[2] * v.e[2];
}

inline vec3 cross(const vec3 &u, const vec3 &v) {
    return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
                u.e[2] * v.e[0] - u.e[0] * v.e[2],
                u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

inline vec3 unit_vector(vec3 v) {
    return v / v.length();
}

//for 04随机获取一个球内点，为了获取漫反射随机光线。
/*****************************************
 * 用拒绝采样方法在单位球内产生随机点
 * 这些点沿着表面法线偏移。这相当于在半球上选择方向，
 * 其在接近法线的地方具有较高的概率，而在掠射角处散射光线的概率较低。
 * 这个分布按照 cos³(𝜙) 进行缩放，其中 𝜙 是与法线的角度。
 * 因为以较小的角度到达的光会在更大的区域上扩散，从而对最终颜色的贡献较低。
 */
vec3 random_in_unit_sphere() {
    while (true) {
        auto p = vec3::random(-1,1);

        //如果向量长度>1说明该点在球外
        if (p.length_squared() >= 1) continue;
        return p;
    }
}


//For 06, create a true lambertian reflection
vec3 random_unit_vector() {
    //单位化
    return unit_vector(random_in_unit_sphere());
}

//07获得反射法线，原理见notion
vec3 reflect(const vec3& v, const vec3& n) {
    return v - 2*dot(v,n)*n;
}

//09Dieletrics，折射函数,返回折射后的光线
vec3 refract(const vec3& uv, const vec3& n, double etai_over_etat) {
    auto cos_theta = fmin(dot(-uv, n), 1.0);
    vec3 r_out_perp =  etai_over_etat * (uv + cos_theta*n);
    vec3 r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.length_squared())) * n;
    return r_out_perp + r_out_parallel;
}

//11Defocus_blur, to generate random point inside the disk center.
vec3 random_in_unit_disk() {
    while (true) {
        auto p = vec3(random_double(-1,1), random_double(-1,1), 0);
        if (p.length_squared() >= 1) continue;
        return p;
    }
}
#endif
