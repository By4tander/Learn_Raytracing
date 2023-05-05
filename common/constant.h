//
// Created by 贾奕人 on 2023/5/5.
//

#ifndef RAYTRACING_CONSTANT_H
#define RAYTRACING_CONSTANT_H

#include <cmath>
#include <limits>
#include <memory>
#include <cstdlib>

//usings
using std::shared_ptr;
using std::make_shared;
using std::sqrt;

//constants
const double infinity=std::numeric_limits<double>::infinity();
const double pi=3.1415926535897;

//utility functions
//角度转化为弧度
inline double degrees_to_radians(double degrees){
    return degrees*pi/180.0;
}

//generate random number
//--------[0,1)----------
inline double random_double(){
    return rand()/(RAND_MAX+1.0);
}
//-----[min,max)---------
inline double random_double(double min, double max){
    return min+(max-min)*random_double();
}

//common headers
#include "ray.h"
#include "vec3.h"

//clamps the value to the range [min,max]:
inline double clamp(double x, double min, double max) {
    if (x < min) return min;
    if (x > max) return max;
    return x;
}
#endif //RAYTRACING_CONSTANT_H
