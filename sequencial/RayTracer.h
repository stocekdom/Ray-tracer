//
// Created by dominik on 20.02.26.
//

#ifndef SEQUENCIAL_RAYTRACER_H
#define SEQUENCIAL_RAYTRACER_H

#include "Color.h"
#include "Objects.h"
#include <vector>

using Pixels = std::vector<Color>;

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
   unsigned int imageWidth;
   unsigned int imageHeight;
   Color backgroundColor;
};

class RayTracer
{
   public:
      RayTracer();

      /**
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
       * @note See more about the generation method in the report: REPORT.md
       *
       * @warning The ray tracer uses the Y axis as up axis.
       *
       * @param options The ray tracer options
       * @param objects A list of objects in a scene
       * @param lights A list of lights in a scene
       * @return A vector of individual pixel colors. The amount is equal to options.imageWidth * options.imageHeight
       */
      Pixels generateImage( const TracerOptions& options, const std::vector<Object>& objects, const std::vector<Light>& lights );

   private:
      static constexpr float MAX_FOV = 120.f;

      struct Viewport
      {
         float width;
         float height;
         float pixelWidth;
         float pixelHeight;
         Vector3f bottomLeftCorner;
      };

      static Viewport calculateViewport( const TracerOptions& options );

      static Ray generateRayForPixel( const TracerOptions& options, const Viewport& viewport,
                                      unsigned int pixelX, unsigned int pixelY );
};
#endif //SEQUENCIAL_RAYTRACER_H
