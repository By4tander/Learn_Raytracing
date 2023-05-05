

cmake_minimum_required(VERSION 3.25)
project(Raytracing)

set(CMAKE_CXX_STANDARD 17)

# Source
set(SRCS
        00Basic
        01Normals
        02Add_Objects
        03Antialiasing
        )



foreach(SRC ${SRCS})
    add_executable(${SRC}
            src/${SRC}.cpp
            common/hittable_list.h
            common/render_color.h
            common/hittable.h
            common/ray.h
            common/sphere.h
            common/vec3.h
            common/constant.h
            common/camera.h
            )
    set_target_properties(${SRC} PROPERTIES CXX_STANDARD 20 CXX_EXTENSIONS ON)



endforeach()
