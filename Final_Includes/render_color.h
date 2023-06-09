//
// Created by 贾奕人 on 2023/5/3.
//

//Rather than adding in a fractional contribution
//each time we accumulate more light to the color,
//just add the full color each iteration,
//and then perform a single divide at the end
// (by the number of samples) when writing out the color.
#ifndef RAYTRACING_RENDER_COLOR_H
#define RAYTRACING_RENDER_COLOR_H

#include "vec3.h"
#include "constant.h"
#include <fstream>

//-------------------Function Before-------------------------
void write_color_simple(std::ofstream &ofs, color pixel_color) {
    // Write the translated [0,255] value of each color component.
    ofs << static_cast<int>(255.999 * pixel_color.x()) << ' '
        << static_cast<int>(255.999 * pixel_color.y()) << ' '
        << static_cast<int>(255.999 * pixel_color.z()) << '\n';
}

//-----------------Antialiasing and---------------
void write_color_with_samples(std::ofstream &out, color pixel_color, int samples_per_pixel) {
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    // Divide the color by the number of samples.
    auto scale = 1.0 / samples_per_pixel;

    //gamma-correct for gamma=2.0
    r = sqrt(scale * r);
    g = sqrt(scale * g);
    b = sqrt(scale * b);

    //without gamma-correct
//    r *= scale;
//    g *= scale;
//    b *= scale;


    // Write the translated [0,255] value of each color component.
    out << static_cast<int>(256 * clamp(r, 0.0, 0.999)) << ' '
        << static_cast<int>(256 * clamp(g, 0.0, 0.999)) << ' '
        << static_cast<int>(256 * clamp(b, 0.0, 0.999)) << '\n';
}

#endif //RAYTRACING_RENDER_COLOR_H
