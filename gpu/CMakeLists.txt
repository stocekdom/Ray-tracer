cmake_minimum_required(VERSION 3.28)
project(GPURayTracer LANGUAGES CXX CUDA)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
set(CMAKE_CUDA_STANDARD 17)
set(CMAKE_CUDA_STANDARD_REQUIRED ON)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CUDA_ARCHITECTURES 89)

find_package(CUDAToolkit REQUIRED)

add_executable(raytracer main.cpp
        Objects.cuh
        Color.cuh
        Vector.cuh
        Material.h
        Color.cu
        Material.cpp
        GPURayTracer.cuh
        GPURayTracer.cu
        Math.cuh
        Objects.cu
        lodepng.h
        lodepng.cpp
        RayTracerKernel.cu
        TracerOptions.h
        Viewport.h
        RayTraceResult.h
        CUDAAnnotations.cuh
        RayTracerKernel.cuh
        GPUArray.cuh
        ErrorHandle.h
)

target_link_libraries(raytracer PRIVATE CUDA::cudart)

set_target_properties(raytracer PROPERTIES
        CUDA_SEPARABLE_COMPILATION ON
)