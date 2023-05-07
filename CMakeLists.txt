

cmake_minimum_required(VERSION 3.25)
project(Raytracing)

set(CMAKE_CXX_STANDARD 20)

#找全局库，在后面加required则找不到就报错
#find_package(OpenCV)

# Source
set(SRCS
        00Basic
        01Normals
        02Add_Objects
        03Antialiasing
        04Simple_DiffuseMaterial
        05Better_Diffuse
        06Lambertian_Reflection
        07Metal
        08Fuzzy_Reflection
        )

foreach(SRC ${SRCS})
    add_executable(${SRC}
            )

    target_sources(${SRC}
            PUBLIC
            src/${SRC}.cpp
            common/hittable_list.h
            common/render_color.h
            common/hittable.h
            common/ray.h
            common/sphere.h
            common/vec3.h
            common/constant.h
            common/camera.h
            common/material.h
            common/Sphere_with_material.h
            )

    set_target_properties(${SRC}
            PROPERTIES CXX_STANDARD 20
            CXX_EXTENSIONS ON)

#    实现直接找到include目录，避免写相对路径
    target_include_directories(${SRC}
            PUBLIC
            common
            )


endforeach()
