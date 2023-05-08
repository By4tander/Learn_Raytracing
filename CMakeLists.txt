

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
        09Dielectrics
        10Positionable_Camera
        11Defocus_Blur
        Final
        )

foreach(SRC ${SRCS})
    add_executable(${SRC}
            )

    target_sources(${SRC}
            PUBLIC
            src/${SRC}.cpp
            Final_Includes/hittable_list.h
            Final_Includes/render_color.h
            Final_Includes/hittable.h
            Final_Includes/ray.h
            common/sphere.h
            Final_Includes/vec3.h
            Final_Includes/constant.h
            common/camera.h
            Final_Includes/material.h
            Final_Includes/Sphere_with_material.h
            common/Positionable_Camera.h
            Final_Includes/Final_Camera.h
            )

    set_target_properties(${SRC}
            PROPERTIES CXX_STANDARD 20
            CXX_EXTENSIONS ON)

#    实现直接找到include目录，避免写相对路径
    target_include_directories(${SRC}
            PUBLIC
            common
            Final_Includes
            )


endforeach()
