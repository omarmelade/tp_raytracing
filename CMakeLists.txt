cmake_minimum_required(VERSION 3.26)
project(tp_synthese_raytracing)

set(CMAKE_CXX_STANDARD 17)

add_executable(tp_synthese_raytracing
        main.cpp
        Sphere.cpp Sphere.h
        Rayon.cpp Rayon.h
        libs/easyppm/easyppm.c libs/easyppm/easyppm.h
        Lampe.cpp
        Lampe.h
)
