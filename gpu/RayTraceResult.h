//
// Created by dominik on 30.03.26.
//

#ifndef GPURAYTRACER_RAYTRACERESULT_H
#define GPURAYTRACER_RAYTRACERESULT_H

#include <cfloat>
#include "Vector.cuh"

struct RayHitResult
{
   float distance = FLT_MAX;
   Vector3f hitPoint;
   Vector3f normal;
};

// Currently we use smart pointers in the vector for objects which gives use scattered memory and bad cache coherence = slower access, but it gives use easy polymorphism and intersection detection
// We could use variant with a variant + visit method, Enum tags or arrays for all types, but this will be used in the CUDA solution
struct RayTraceResult
{
   RayHitResult closestHit{};
   int closestHitObjectIndex = -1;
};

#endif //GPURAYTRACER_RAYTRACERESULT_H
