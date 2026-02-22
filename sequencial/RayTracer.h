//
// Created by dominik on 20.02.26.
//

#ifndef SEQUENCIAL_RAYTRACER_H
#define SEQUENCIAL_RAYTRACER_H

#include <memory>

#include "Color.h"
#include "Objects.h"
#include <vector>

using Pixels = std::vector<Color>;
using RawPixels = std::vector<unsigned char>;

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
      static Pixels generateImage( const TracerOptions& options, const std::vector<std::shared_ptr<SceneObject>>& objects,
                                   const std::vector<Light>& lights );

      /**
       * @brief An overload of the generateImage method which returns raw pixel data, that can be used directly with Png libraries
       * @param options The ray tracer options
       * @param objects A list of objects in a scene
       * @param lights A list of lights in a scene
       * @return A vector of individual pixel data. Each element represents one color channel, and the pixels are stored behind each other in memory as unsigned chars.
       * E.g. data: R,G,B,A,R,G,B,A The amount is equal to options.imageWidth * options.imageHeight
       */
      static RawPixels generateRawImage( const TracerOptions& options, const std::vector<std::shared_ptr<SceneObject>>& objects,
                                         const std::vector<Light>& lights );

   private:
      static constexpr float MAX_FOV = 120.f;
      static constexpr int RGBABytes = 4;

      struct Viewport
      {
         float width;
         float height;
         float pixelWidth;
         float pixelHeight;
         Vector3f bottomLeftCorner;
      };

      // Currently we use smart pointers in the vector for objects which gives use scattered memory and bad cache coherence = slower access, but it gives use easy polymorphism and intersection detection
      // We could use variant with a variant + visit method, Enum tags or arrays for all types, but this will be used in the CUDA solution
      struct RayTraceResult
      {
         RayHitResult closestHit{};
         std::shared_ptr<SceneObject> closestObject{};
      };

      static Viewport calculateViewport( const TracerOptions& options );

      static RayTraceResult traceRay( const Ray& ray, const std::vector<std::shared_ptr<SceneObject>>& objects );

      static Ray generateRayForPixel( const TracerOptions& options, const Viewport& viewport,
                                      unsigned int pixelX, unsigned int pixelY );

      static void addColorToRawPixels( RawPixels& rawPixels, const Color& color, size_t index );
};
#endif //SEQUENCIAL_RAYTRACER_H
