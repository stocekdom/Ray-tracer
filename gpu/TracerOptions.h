//
// Created by dominik on 30.03.26.
//

#ifndef GPURAYTRACER_TRACEROPTIONS_H
#define GPURAYTRACER_TRACEROPTIONS_H
#include "Color.cuh"

/**
 * @brief Defines the options for generating a ray-traced image
 * The viewport is defined using cameraDistance and fieldOfView. The field of view is used to determine the width of the viewport based on the distance.
 * The viewport height is then calculated from the aspect ratio of the image height and width
 * The eye position is at (0,0,0), and the viewport center is aligned with the eye on the x-axis, so the viewport center C is at (distance,0,0)
 * The viewport calculated from the FOV and distance is then filled with the desired number of pixels.
 * During ray-tracing the rays are cast in the direction of the eye and a pixel center
 */
struct TracerOptions
{
   float cameraDistance;
   float fieldOfView;
   unsigned int maxRecursionDepth;
   unsigned int imageWidth;
   unsigned int imageHeight;
   Color backgroundColor;
   Color ambientLightColor;
};

#endif //GPURAYTRACER_TRACEROPTIONS_H
