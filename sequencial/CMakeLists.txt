cmake_minimum_required(VERSION 3.28)
project(sequencial)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

add_executable(sequencial main.cpp
        Objects.h
        Color.h
        Vector.h
        Material.h
        Color.cpp
        Material.cpp
        RayTracer.h
        RayTracer.cpp
        Math.h
        Objects.cpp
        lodepng.h
        lodepng.cpp
)
