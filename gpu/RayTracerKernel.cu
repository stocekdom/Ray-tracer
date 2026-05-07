//
// Created by dominik on 30.03.26.
//

#include "RayTracerKernel.cuh"
#include "CUDAAnnotations.h"
#include "RayTraceResult.h"
#include "Viewport.h"

static constexpr float SHADOW_RAY_OFFSET = 0.05f;

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

bool isValidObjectIndex( int objectIndex, const ReadOnlyGPUArrayView<SceneObject>& objects )
{
   return objectIndex >= 0 && objectIndex < objects.size();
}

Ray generateRayForPixel( const TracerOptions& options, const Viewport& viewport, unsigned int pixelX, unsigned int pixelY )
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
   Vector3f pixelCoords( pixelCenterX, pixelCenterY, options.cameraDistance );
   pixelCoords.normalize();
   return Ray{ Vector3f( 0.f, 0.f, 0.f ), pixelCoords };
}

Ray generateShadowRay( const Light& light, const Vector3f& intersectionPoint )
{
   auto rayDirection = light.centerPosition - intersectionPoint;
   rayDirection.normalize();
   return { intersectionPoint, rayDirection };
}

/**
 * Returns the reflexion color of the closest object using the Blinn-Phong reflexion model
 *
 * @warning This function doesn't check if the path to the light is obstructed or not
 *
 * @param light Current light to calculate the reflexion for
 * @param closestResult The result of the original ray trace
 * @param originalRay The original ray used to get the intersection point of an object
 * @param objects A list of objects in the scene
 * @return
 */
Color blinnPhongReflexion( const Light& light, const RayTraceResult& closestResult,
                           const Ray& originalRay, const ReadOnlyGPUArrayView<SceneObject>& objects )
{
   auto offsetHitPoint = closestResult.closestHit.hitPoint + closestResult.closestHit.normal * SHADOW_RAY_OFFSET;
   auto lightRay = generateShadowRay( light, offsetHitPoint );
   auto lightDistance = offsetHitPoint.getEuclideanDistance( light.centerPosition );
   auto& material = objects[ closestResult.closestHitObjectIndex ].material;

   // Trace a ray from the closest objects intersect point to the light
   auto lightTraceResult = traceRay( lightRay, objects );

   if( isValidObjectIndex( lightTraceResult.closestHitObjectIndex, objects ) &&
       lightTraceResult.closestHit.distance < lightDistance )
      return {};

   auto distance = closestResult.closestHit.hitPoint.getEuclideanDistance( light.centerPosition );
   auto distanceAttenuation = light.intensity / ( distance * distance );

   // Here, the direction of the light ray is normalized
   auto diffuse = light.lightColor *
                  max( 0.0f, VectorOps::dotProduct( lightRay.direction, closestResult.closestHit.normal ) ) *
                  material.diffuseColor;
   // Using Blinn halfway vector. We use '-' since the original ray is from the eye, and we need it reversed. Whole formula: lighDir + (-origRayDir)
   auto halfwayVector = lightRay.direction - originalRay.direction;
   halfwayVector.normalize();

   auto shininessPart = powf( max( 0.0f, VectorOps::dotProduct( closestResult.closestHit.normal, halfwayVector ) ),
                              material.shininess );
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
Color getRayTracedColor( const TracerOptions& options, const Ray& ray,
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

__global__ void rayTracerKernel( const ReadOnlyGPUArrayView<SceneObject>& objects,
                                 const ReadOnlyGPUArrayView<Light>& lights,
                                 const TracerOptions& options,
                                 GPUArrayView<Color>& output )
{
   // TODO for cycle for pixels + Add colors to a resulting array
}

void launchKernel( const ReadOnlyGPUArrayView<SceneObject>& objects,
                   const ReadOnlyGPUArrayView<Light>& lights,
                   const TracerOptions& options,
                   GPUArrayView<Color>& output )
{
   rayTracerKernel<<<32,256>>>( objects, lights, options, output );
}
