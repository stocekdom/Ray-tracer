cmake_minimum_required(VERSION 4.1)
project(sequencial)

set(CMAKE_CXX_STANDARD 20)

add_executable(sequencial main.cpp
        Objects.h
        Color.h
        Vector.h
        Material.h
        Color.cpp
        Material.cpp
        RayTracer.h
        RayTracer.cpp
)
