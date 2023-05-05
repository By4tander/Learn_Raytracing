//
// Created by 贾奕人 on 2023/5/3.
//

#ifndef RAYTRACING_RENDER_COLOR_H
#define RAYTRACING_RENDER_COLOR_H

#include "vec3.h"

#include <fstream>

void write_color(std::ofstream &ofs, color pixel_color) {
    // Write the translated [0,255] value of each color component.
    ofs << static_cast<int>(255.999 * pixel_color.x()) << ' '
        << static_cast<int>(255.999 * pixel_color.y()) << ' '
        << static_cast<int>(255.999 * pixel_color.z()) << '\n';
}

#endif //RAYTRACING_RENDER_COLOR_H
