//
// Created by dominik on 30.03.26.
//

#ifndef GPURAYTRACER_VIEWPORT_H
#define GPURAYTRACER_VIEWPORT_H

#include "Vector.cuh"

struct Viewport
{
   float width;
   float height;
   float pixelWidth;
   float pixelHeight;
   Vector3f bottomLeftCorner;
};

#endif //GPURAYTRACER_VIEWPORT_H
