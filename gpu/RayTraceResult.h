//
// Created by dominik on 30.03.26.
//

#ifndef GPURAYTRACER_RAYTRACERESULT_H
#define GPURAYTRACER_RAYTRACERESULT_H

#include <cfloat>
#include <vector_types.h>

struct RayHitResult
{
   float distance = FLT_MAX;
   float4 hitPoint{};
   float4 normal{};
};

struct RayTraceResult
{
   RayHitResult closestHit{};
   int closestHitObjectIndex = -1;
};

#endif //GPURAYTRACER_RAYTRACERESULT_H
