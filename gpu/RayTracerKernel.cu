//
// Created by dominik on 30.03.26.
//
#include <cuda_runtime.h>

#include "Objects.h"
#include "TracerOptions.h"
#include "Viewport.h"

void launchKernel( )
{

}

__global__ void rayTracerKernel()
{

}


GPURayTracer::RayTraceResult GPURayTracer::traceRay( const Ray& ray, const std::vector<std::shared_ptr<SceneObject>>& objects )
{
   RayHitResult closestResult;
   std::shared_ptr<SceneObject> closestObject = nullptr;

   for( const auto& object: objects )
   {
      RayHitResult result;
      if( object->intersects( ray, result ) && result.distance < closestResult.distance )
      {
         closestResult = result;
         closestObject = object;
      }
   }

   return { closestResult, closestObject };
}

Ray generateRayForPixel( const TracerOptions& options, const Viewport& viewport, unsigned int pixelX, unsigned int pixelY )
{
   // Flip Y: image row 0 is the top row in most image formats/viewers,
   // but our viewport math starts from the bottom-left corner.
   unsigned int flippedY = ( options.imageHeight - 1u ) - pixelY;
   // We add the viewport.pixelWidth / 2 to get to the center of the pixel
   float pixelCenterX = viewport.bottomLeftCorner.x + ( viewport.pixelWidth * static_cast<float>( pixelX ) ) +
                        viewport.pixelWidth / 2.0f;
   float pixelCenterY = viewport.bottomLeftCorner.y + ( viewport.pixelHeight * static_cast<float>( flippedY ) ) +
                        viewport.pixelHeight / 2.0f;

   // Create the pixel coordinates that also act as a ray direction vector since the eye is at 0,0,0 and the direction is P - E
   Vector3f pixelCoords( pixelCenterX, pixelCenterY, options.cameraDistance );
   pixelCoords.normalize();
   return Ray{ Vector3f( 0.f, 0.f, 0.f ), pixelCoords };
}

Ray GPURayTracer::generateShadowRay( const Light& light, const Vector3f& intersectionPoint )
{
   auto rayDirection = light.centerPosition - intersectionPoint;
   rayDirection.normalize();
   return { intersectionPoint, rayDirection };
}

Color GPURayTracer::getRayTracedColor( const TracerOptions& options, const Ray& ray,
                                    const std::vector<std::shared_ptr<SceneObject>>& objects, const std::vector<Light>& lights )
{
   auto traceResult = traceRay( ray, objects );
   auto& material = traceResult.closestObject->material;

   if( !traceResult.closestObject )
      return options.backgroundColor;

   // Object shading
   // Start with ambient color (intensity)
   Color finalColor = material.baseColor * options.ambientLightColor;

   // Blinn-Phong model
   for( const auto& light: lights )
      finalColor += blinnPhongReflexion( light, traceResult, ray, objects );

   return finalColor;
}

void GPURayTracer::addColorToRawPixels( RawPixels& rawPixels, const Color& color, size_t index )
{
   // TODO tone map
   rawPixels[ index ] = toneMapToUint8( color.R );
   rawPixels[ index + 1 ] = toneMapToUint8( color.G );
   rawPixels[ index + 2 ] = toneMapToUint8( color.B );
   rawPixels[ index + 3 ] = color.alpha;
}

Color GPURayTracer::blinnPhongReflexion( const Light& light, const RayTraceResult& closestResult,
                                      const Ray& originalRay, const std::vector<std::shared_ptr<SceneObject>>& objects )
{
   auto offsetHitPoint = closestResult.closestHit.hitPoint + closestResult.closestHit.normal * SHADOW_RAY_OFFSET;
   auto lightRay = generateShadowRay( light, offsetHitPoint );
   auto lightDistance = offsetHitPoint.getEuclideanDistance( light.centerPosition );
   auto& material = closestResult.closestObject->material;

   // Trace a ray from the closest objects intersect point to the light
   auto lightTraceResult = traceRay( lightRay, objects );

   if( lightTraceResult.closestObject && lightTraceResult.closestHit.distance < lightDistance )
      return {};

   auto distance = closestResult.closestHit.hitPoint.getEuclideanDistance( light.centerPosition );
   auto distanceAttenuation = light.intensity / ( distance * distance );
   // Here, the direction of the light ray is normalized
   auto diffuse = light.lightColor *
                  std::max( 0.0f, Math::dotProduct( lightRay.direction, closestResult.closestHit.normal ) ) *
                  material.diffuseColor;
   // Using Blinn halfway vector. We use '-' since the original ray is from the eye, and we need it reversed. Whole formula: lighDir + (-origRayDir)
   auto halfwayVector = lightRay.direction - originalRay.direction;
   halfwayVector.normalize();

   auto shininessPart = std::pow( std::max( 0.0f, Math::dotProduct( closestResult.closestHit.normal, halfwayVector ) ),
                                  material.shininess );
   auto specular = light.lightColor * shininessPart * material.specular;

   return ( diffuse + specular ) * distanceAttenuation;
}

uint8_t GPURayTracer::toneMapToUint8( float value )
{
   // Can also use gamma correction, but that seems to bright
   // TODO magical number for exposure fix
   return static_cast<uint8_t>( std::lround( Math::gammaCorrection( Math::exposureToneMapping( value, 1.1f ), 1.6f ) * 255.0f ) );
}
