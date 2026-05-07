//
// Created by dominik on 31.03.26.
//

#ifndef GPURAYTRACER_RAYTRACERKERNEL_CUH
#define GPURAYTRACER_RAYTRACERKERNEL_CUH

#include "GPUArray.h"
#include "Objects.cuh"
#include "TracerOptions.h"

void launchKernel( const ReadOnlyGPUArrayView<SceneObject>& objects,
                   const ReadOnlyGPUArrayView<Light>& lights,
                   const TracerOptions& options,
                   GPUArrayView<Color>& output );

#endif //GPURAYTRACER_RAYTRACERKERNEL_CUH
