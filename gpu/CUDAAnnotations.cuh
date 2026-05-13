//
// Created by dominik on 31.03.26.
//

#ifndef GPURAYTRACER_CUDAANNOTATIONS_H
#define GPURAYTRACER_CUDAANNOTATIONS_H

#include <cuda_runtime.h>

#define GPU_HD __host__ __device__
#define GPU_DEV __device__
#define HOST_DEV __host__
#define GPU_KERNEL __global__

#endif //GPURAYTRACER_CUDAANNOTATIONS_H
