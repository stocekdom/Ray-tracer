//
// Created by dominik on 30.03.26.
//

#include "RayTracerKernel.cuh"
#include "CUDAAnnotations.cuh"
#include "RayTraceResult.h"
#include <c++/13/iostream>

#define BLOCK_SIZE 512

static constexpr float SHADOW_RAY_OFFSET = 0.05f;

/**
 * Trace a ray and test it against all objects in a scene
 * @param ray The ray
 * @param objects List of all objects
 * @return The result containing the hit point, distance to it, and the index of an object that got hit
 */
GPU_DEV RayTraceResult traceRay( const Ray& ray, const ReadOnlyGPUArrayView<SceneObject> objects )
{
   RayHitResult closestResult;
   int closestObjectIndex = -1;

   for( int i = 0; i < objects.size(); ++i )
   {
      RayHitResult result;
      if( objects[ i ].intersects( ray, result ) && result.distance < closestResult.distance )
      {
         closestResult = result;
         closestObjectIndex = i;
      }
   }

   return { closestResult, closestObjectIndex };
}

GPU_DEV bool isValidObjectIndex( int objectIndex, const ReadOnlyGPUArrayView<SceneObject>& objects )
{
   return objectIndex >= 0 && objectIndex < objects.size();
}

/**
 * Calculates the ray coordinates based on the pixel and viewport dimensions
 * @param options Ray tracer options
 * @param viewport The viewport
 * @param pixelX Pixel index that the ray intersects on the x-axis
 * @param pixelY Pixel index that the ray intersects on the y-axis
 * @return Constructed ray for pixel X, Y
 */
GPU_DEV Ray generateRayForPixel( const TracerOptions& options, const Viewport& viewport, unsigned int pixelX,
                                 unsigned int pixelY )
{
   // Flip Y: image row 0 is the top row in most image formats/viewers,
   // but our viewport math starts from the bottom-left corner.
   pixelY = ( options.imageHeight - 1u ) - pixelY;
   // We add the viewport.pixelWidth / 2 to get to the center of the pixel
   float pixelCenterX = viewport.bottomLeftCorner.x + ( viewport.pixelWidth * static_cast<float>( pixelX ) ) +
                        viewport.pixelWidth / 2.0f;
   float pixelCenterY = viewport.bottomLeftCorner.y + ( viewport.pixelHeight * static_cast<float>( pixelY ) ) +
                        viewport.pixelHeight / 2.0f;

   // Create the pixel coordinates that also act as a ray direction vector since the eye is at 0,0,0 and the direction is P - E
   float4 pixelCoords{ pixelCenterX, pixelCenterY, options.cameraDistance, 0.f };
   normalize3( pixelCoords );
   return Ray{ float4{ 0.f, 0.f, 0.f, 0.f }, pixelCoords };
}

/**
 * Generates a shadow ray from the scene object intersection point with a ray and a light source
 * @param light A light source
 * @param intersectionPoint Intersection point of a ray and a scene object
 * @return Constructed ray from the intersection point to the light source
 */
GPU_DEV Ray generateShadowRay( const Light& light, const float4& intersectionPoint )
{
   auto rayDirection = light.centerPosition - intersectionPoint;
   normalize3( rayDirection );
   return { intersectionPoint, rayDirection };
}

/**
 * Returns the reflexion color of the closest object using the Blinn-Phong reflexion model
 *
 * @param light Current light to calculate the reflexion for
 * @param closestResult The result of the original ray trace
 * @param originalRay The original ray used to get the intersection point of an object
 * @param objects A list of objects in the scene
 * @return
 */
GPU_DEV Color blinnPhongReflexion( const Light& light, const RayTraceResult& closestResult,
                                   const Ray& originalRay, const ReadOnlyGPUArrayView<SceneObject>& objects )
{
   auto offsetHitPoint = closestResult.closestHit.hitPoint + closestResult.closestHit.normal * SHADOW_RAY_OFFSET;
   auto lightRay = generateShadowRay( light, offsetHitPoint );
   auto lightDistance = euclideanDistance3( offsetHitPoint, light.centerPosition );
   auto& material = objects[ closestResult.closestHitObjectIndex ].material;

   // Trace a ray from the closest objects intersect point to the light
   // This determines if the hit point is in the shadow and doesn't have any additional color
   auto lightTraceResult = traceRay( lightRay, objects );

   if( isValidObjectIndex( lightTraceResult.closestHitObjectIndex, objects ) &&
       lightTraceResult.closestHit.distance < lightDistance )
      return {};

   auto distance = euclideanDistance3( closestResult.closestHit.hitPoint, light.centerPosition );
   auto distanceAttenuation = light.intensity / ( distance * distance );

   // Here, the direction of the light ray is normalized
   auto diffuse = light.lightColor
                  * fmaxf( 0.0f, dotProduct3( lightRay.direction, closestResult.closestHit.normal ) )
                  * material.diffuseColor;

   // Using Blinn halfway vector. We use '-' since the original ray is from the eye, and we need it reversed. Whole formula: lighDir + (-origRayDir)
   auto halfwayVector = lightRay.direction - originalRay.direction;
   normalize3( halfwayVector );

   auto shininessPart = powf( fmaxf( 0.0f, dotProduct3( closestResult.closestHit.normal, halfwayVector ) ), material.shininess );
   auto specular = light.lightColor * shininessPart * material.specular;

   return ( diffuse + specular ) * distanceAttenuation;
}

/**
 *
 * Traces a ray and returns a final color this ray generates.
 * @param options The ray tracer parameters
 * @param ray The ray to trace
 * @param objects A list of objects in the scene
 * @param lights A list of lights in the scene
 * @return Returns
 */
GPU_DEV Color getRayTracedColor( const TracerOptions& options, const Ray& ray,
                                 const ReadOnlyGPUArrayView<SceneObject>& objects,
                                 const ReadOnlyGPUArrayView<Light>& lights )
{
   auto traceResult = traceRay( ray, objects );

   if( !isValidObjectIndex( traceResult.closestHitObjectIndex, objects ) )
      return options.backgroundColor;

   auto& material = objects[ traceResult.closestHitObjectIndex ].material;
   // Object shading
   // Start with ambient color (intensity)
   Color finalColor = material.baseColor * options.ambientLightColor;

   // Blinn-Phong model
   for( const auto& light: lights )
      finalColor += blinnPhongReflexion( light, traceResult, ray, objects );

   return finalColor;
}

GPU_KERNEL void rayTracerKernel( ReadOnlyGPUArrayView<SceneObject> objects,
                                 ReadOnlyGPUArrayView<Light> lights,
                                 TracerOptions options,
                                 Viewport viewport,
                                 GPUArrayView<Color> output )
{
   auto id = blockDim.x * blockIdx.x + threadIdx.x;

   if( id >= options.imageWidth * options.imageHeight )
      return;

   output[ id ] = getRayTracedColor( options,
                                     generateRayForPixel( options, viewport, id % options.imageWidth, id / options.imageWidth ),
                                     objects, lights );
}

void launchKernel( const ReadOnlyGPUArrayView<SceneObject>& objects,
                   const ReadOnlyGPUArrayView<Light>& lights,
                   const TracerOptions& options,
                   const Viewport& viewport,
                   GPUArrayView<Color> output )
{
   auto work = options.imageWidth * options.imageHeight;
   auto blocks = ( work + BLOCK_SIZE - 1 ) / BLOCK_SIZE;

   rayTracerKernel<<<blocks,BLOCK_SIZE>>>( objects, lights, options, viewport, output );

   cudaError_t err = cudaGetLastError();
   if( err != cudaSuccess )
   {
      std::cerr << "Kernel launch error: "
            << cudaGetErrorString( err )
            << std::endl;
   }
}
