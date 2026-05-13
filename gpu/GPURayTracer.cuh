//
// Created by dominik on 20.02.26.
//

#ifndef GPURAYTRACER_RAYTRACER_H
#define GPURAYTRACER_RAYTRACER_H

#include "Objects.cuh"
#include "TracerOptions.h"
#include "Viewport.h"
#include <memory>
#include <vector>

using Pixels = std::vector<Color>;
using RawPixels = std::vector<unsigned char>;

class GPURayTracer
{
   public:
      // TODO add tone mapping
      /**
       * @deprecated Doesn't use tone mapping for now. Use generateRawImage instead
       *
       * @brief Generates a ray-traced image from the objects, lights, and defined options
       *
       * @details Process of ray-tracing:
       * 1. The dimensions of the viewport are calculated using the FOV and distance to determine the viewport size in the world. The viewport is then filled with the number of pixels in options.
       * 2. For each pixel of the viewport, we cast a ray into the world with a direction derived from the pixel center and the eye (E) position (normalize(P[i,j] - E))
       * 3. If the ray hits an object, we take the closest intersection point and cast a shadow ray from it to all lights. If all lights are blocked, we use materials ambient color as pixel color.
       * 4. If the ray hits the light, we calculate the pixel color using the Blinn-Phong model. We use the objects material diffuse, specular, and shininess together with the light color to calculate the color.
       * We also use the distance to the light to attenuate it.
       * 5. If the material is reflective, we cast another reflective ray recursively until a non-reflective material is found or the ray doesn't hit anything.
       * This new ray acts as a regular ray, and the color of its intersection point is also calculated using the Blinn-Phong model. The resulting color is added back to the original intersection point
       * 6. If the material is refractive, we cast a refraction ray using the Schnell law. This ray also acts as a regular ray, and we add the resulting color back to the original model
       *
       * @remarks This ray tracer uses basic ray-tracing without any optimizations and no space partitioning.
       * For each pixel, a ray is cast and is checked with all scene objects, and a shadow ray made from all intersection points is checked with all lights
       *
       * @note See more about the generation method in the report: REPORT.md
       *
       * @warning The ray tracer uses the Y axis as up axis.
       *
       * @param options The ray tracer options
       * @param objects A list of objects in a scene
       * @param lights A list of lights in a scene
       * @return A vector of individual pixel colors. The amount is equal to options.imageWidth * options.imageHeight
       */
      static Pixels generateImage( const TracerOptions& options, const std::vector<SceneObject>& objects,
                                   const std::vector<Light>& lights );

      /**
       * @brief An overload of the generateImage method which returns raw pixel data, that can be used directly with Png libraries
       * @param options The ray tracer options
       * @param objects A list of objects in a scene
       * @param lights A list of lights in a scene
       * @return A vector of individual pixel data. Each element represents one color channel, and the pixels are stored behind each other in memory as unsigned chars.
       * E.g. data: R,G,B,A,R,G,B,A The amount is equal to options.imageWidth * options.imageHeight
       */
      static RawPixels generateRawImage( const TracerOptions& options, const std::vector<SceneObject>& objects,
                                         const std::vector<Light>& lights );

   private:
      static constexpr float MAX_FOV = 120.f;
      static constexpr int RGBABytes = 4;
      // Determines how much of the intersection point normal vector is added to the intersection point to offset it from the original intersection point.
      // This avoids self-intersections and fixes the "shadow acne"

      static Viewport calculateViewport( const TracerOptions& options );

      static void addColorToRawPixels( RawPixels& rawPixels, const Color& color, size_t index );

      static uint8_t toneMapToUint8( float value );
};
#endif //GPURAYTRACER_RAYTRACER_H
