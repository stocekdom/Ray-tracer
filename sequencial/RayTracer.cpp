//
// Created by dominik on 20.02.26.
//
#include "RayTracer.h"
#include "Math.h"

Pixels RayTracer::generateImage( const TracerOptions& options, const std::vector<std::shared_ptr<SceneObject>>& objects,
                                 const std::vector<Light>& lights )
{
   float clampedFOV = std::clamp( options.fieldOfView, 0.0f, MAX_FOV );
   auto viewport = calculateViewport( {
      options.cameraDistance, clampedFOV, options.maxRecursionDepth, options.imageWidth, options.imageHeight,
      options.backgroundColor, options.ambientLightColor
   } );

   Pixels pixels;
   pixels.reserve( options.imageWidth * options.imageHeight );

   for( auto i = 0u; i < options.imageHeight; ++i )
   {
      for( auto j = 0u; j < options.imageWidth; ++j )
      {
         auto ray = generateRayForPixel( options, viewport, j, i );
         pixels.emplace_back( getRayTracedColor( options, ray, objects, lights ) );
      }
   }

   return pixels;
}

RawPixels RayTracer::generateRawImage( const TracerOptions& options, const std::vector<std::shared_ptr<SceneObject>>& objects,
                                       const std::vector<Light>& lights )
{
   float clampedFOV = std::clamp( options.fieldOfView, 0.0f, MAX_FOV );
   auto viewport = calculateViewport( {
      options.cameraDistance, clampedFOV, options.maxRecursionDepth, options.imageWidth, options.imageHeight,
      options.backgroundColor, options.ambientLightColor
   } );

   RawPixels pixels( options.imageWidth * options.imageHeight * RGBABytes );

   for( auto i = 0u; i < options.imageHeight; ++i )
   {
      for( auto j = 0u; j < options.imageWidth; ++j )
      {
         // Generate a ray for the current pixel and trace it
         auto ray = generateRayForPixel( options, viewport, j, i );

         addColorToRawPixels( pixels, getRayTracedColor( options, ray, objects, lights ),
                              ( i * options.imageWidth + j ) * RGBABytes );
      }
   }

   return pixels;
}

RayTracer::Viewport RayTracer::calculateViewport( const TracerOptions& options )
{
   float halfWidth = options.cameraDistance * std::tan( options.fieldOfView / 2.0f );
   float aspectRatio = static_cast<float>( options.imageHeight ) / static_cast<float>( options.imageWidth );
   float viewportHeight = halfWidth * aspectRatio * 2;
   float viewportWidth = halfWidth * 2;

   return {
      viewportWidth,
      viewportHeight,
      viewportWidth / static_cast<float>( options.imageWidth ),
      viewportHeight / static_cast<float>( options.imageHeight ),
      { -halfWidth, -( viewportHeight / 2.0f ), options.cameraDistance }
   };
}

RayTracer::RayTraceResult RayTracer::traceRay( const Ray& ray, const std::vector<std::shared_ptr<SceneObject>>& objects )
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

   closestResult.hitPoint = ray.startPoint + ( closestResult.distance * ray.direction );
   return { closestResult, closestObject };
}

Ray RayTracer::generateRayForPixel( const TracerOptions& options, const Viewport& viewport, unsigned int pixelX,
                                    unsigned int pixelY )
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

Ray RayTracer::generateShadowRay( const Light& light, const Vector3f& intersectionPoint )
{
   auto rayDirection = light.centerPosition - intersectionPoint;
   rayDirection.normalize();
   return { intersectionPoint, rayDirection };
}

Color RayTracer::getRayTracedColor( const TracerOptions& options, const Ray& ray,
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

void RayTracer::addColorToRawPixels( RawPixels& rawPixels, const Color& color, size_t index )
{
   // TODO tone map
   rawPixels[ index ] = toneMapToUint8( color.R );
   rawPixels[ index + 1 ] = toneMapToUint8( color.G );
   rawPixels[ index + 2 ] = toneMapToUint8( color.B );
   rawPixels[ index + 3 ] = color.alpha;
}

Color RayTracer::blinnPhongReflexion( const Light& light, const RayTraceResult& closestResult,
                                      const Ray& originalRay, const std::vector<std::shared_ptr<SceneObject>>& objects )
{
   auto lightRay = generateShadowRay( light, closestResult.closestHit.hitPoint );
   lightRay.direction *= SHADOW_RAY_OFFSET;
   auto& material = closestResult.closestObject->material;

   // Trace a ray from the closest objects intersect point to the light
   auto lightTraceResult = traceRay( lightRay, objects );

   if( lightTraceResult.closestObject )
      return {};

   auto distance = closestResult.closestHit.hitPoint.getEuclideanDistance( light.centerPosition );
   auto distanceAttenuation = light.intensity / ( distance * distance );
   // Here, direction of the light ray is normalized
   auto diffuse = light.lightColor * std::max(
                     0.0f, Math::dotProduct( lightRay.direction, closestResult.closestHit.normal ) ) *
                  distanceAttenuation * material.diffuse * material.baseColor;
   // Using Blinn halfway vector
   auto halfwayVector = lightRay.direction + originalRay.direction;
   halfwayVector.normalize();

   auto shininessPart = std::pow( std::max( 0.0f, Math::dotProduct( lightRay.direction, halfwayVector ) ),
                                  material.shininess );
   auto specular = light.lightColor * shininessPart * distanceAttenuation * material.specular;

   return diffuse + specular;
}

uint8_t RayTracer::toneMapToUint8( float value )
{
   return static_cast<uint8_t>( std::lround( Math::acesFilmicToneMapping( value ) * 255.0f ) );
}
