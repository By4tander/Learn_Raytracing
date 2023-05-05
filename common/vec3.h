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

vec3 random_in_unit_sphere() {
    while (true) {
        auto p = vec3::random(-1,1);

        //如果向量长度>1说明该点在球外
        if (p.length_squared() >= 1) continue;
        return p;
    }
}

#endif
