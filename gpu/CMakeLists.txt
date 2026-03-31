cmake_minimum_required(VERSION 3.28)
project(GPURayTracer LANGUAGES CXX CUDA)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

find_package(CUDAToolkit REQUIRED)

add_executable(raytracer main.cpp
        Objects.h
        Color.h
        Vector.h
        Material.h
        Color.cpp
        Material.cpp
        GPURayTracer.h
        GPURayTracer.cpp
        Math.h
        Objects.cpp
        lodepng.h
        lodepng.cpp
        RayTracerKernel.cu
        TracerOptions.h
        Viewport.h
        RayTraceResult.h
        CUDAAnnotations.h
)

set_target_properties(raytracer PROPERTIES CUDA_ARCHITECTURES 75)