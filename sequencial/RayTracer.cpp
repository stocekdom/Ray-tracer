//
// Created by dominik on 20.02.26.
//
#include "RayTracer.h"

#include <iostream>

Pixels RayTracer::generateImage( const TracerOptions& options, const std::vector<std::shared_ptr<SceneObject>>& objects,
                                 const std::vector<Light>& lights )
{
   float clampedFOV = std::clamp( options.fieldOfView, 0.0f, MAX_FOV );
   auto viewport = calculateViewport( {
      options.cameraDistance, clampedFOV, options.imageWidth, options.imageHeight, options.backgroundColor
   } );

   Pixels pixels;
   pixels.reserve( options.imageWidth * options.imageHeight );

   for( auto i = 0u; i < options.imageHeight; ++i )
   {
      for( auto j = 0u; j < options.imageWidth; ++j )
      {
         // Generate a ray for the current pixel and trace it
         auto result = traceRay( generateRayForPixel( options, viewport, i, j ), objects );

         if( !result.closestObject )
            pixels.emplace_back( options.backgroundColor );
         else
            pixels.emplace_back( result.closestObject->material.baseColor );
      }
   }

   return pixels;
}

RawPixels RayTracer::generateRawImage( const TracerOptions& options, const std::vector<std::shared_ptr<SceneObject>>& objects,
                                       const std::vector<Light>& lights )
{
   float clampedFOV = std::clamp( options.fieldOfView, 0.0f, MAX_FOV );
   auto viewport = calculateViewport( {
      options.cameraDistance, clampedFOV, options.imageWidth, options.imageHeight, options.backgroundColor
   } );

   RawPixels pixels;
   pixels.reserve( options.imageWidth * options.imageHeight );

   for( auto i = 0u; i < options.imageHeight; ++i )
   {
      for( auto j = 0u; j < options.imageWidth; ++j )
      {
         // Generate a ray for the current pixel and trace it
         auto result = traceRay( generateRayForPixel( options, viewport, j, i ), objects );

         if( !result.closestObject )
            emplaceColorToRawPixels( pixels, options.backgroundColor );
         else
            emplaceColorToRawPixels( pixels, result.closestObject->material.baseColor );
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

   return { closestResult, closestObject };
}

Ray RayTracer::generateRayForPixel( const TracerOptions& options, const Viewport& viewport, unsigned int pixelX,
                                    unsigned int pixelY )
{
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

void RayTracer::emplaceColorToRawPixels( RawPixels& rawPixels, const Color& color )
{
   rawPixels.emplace_back( color.R );
   rawPixels.emplace_back( color.G );
   rawPixels.emplace_back( color.B );
   rawPixels.emplace_back( color.alpha );
}
