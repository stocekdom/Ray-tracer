//
// Created by dominik on 20.02.26.
//
#include "GPURayTracer.h"
#include "Math.h"

Pixels GPURayTracer::generateImage( const TracerOptions& options, const std::vector<std::shared_ptr<SceneObject>>& objects,
                                    const std::vector<Light>& lights )
{
   float clampedFOV = std::clamp( options.fieldOfView, 0.0f, MAX_FOV );
   auto viewport = calculateViewport( {
      options.cameraDistance, clampedFOV, options.maxRecursionDepth, options.imageWidth, options.imageHeight,
      options.backgroundColor, options.ambientLightColor
   } );

   Pixels pixels;
   pixels.reserve( options.imageWidth * options.imageHeight );

   // TODO do on GPU
   /*
   for( auto i = 0u; i < options.imageHeight; ++i )
   {
      for( auto j = 0u; j < options.imageWidth; ++j )
      {
         auto ray = generateRayForPixel( options, viewport, j, i );
         pixels.emplace_back( getRayTracedColor( options, ray, objects, lights ) );
      }
   }
   */
   return pixels;
}

RawPixels GPURayTracer::generateRawImage( const TracerOptions& options, const std::vector<std::shared_ptr<SceneObject>>& objects,
                                          const std::vector<Light>& lights )
{
   float clampedFOV = std::clamp( options.fieldOfView, 0.0f, MAX_FOV );
   auto viewport = calculateViewport( {
      options.cameraDistance, clampedFOV, options.maxRecursionDepth, options.imageWidth, options.imageHeight,
      options.backgroundColor, options.ambientLightColor
   } );

   RawPixels pixels( options.imageWidth * options.imageHeight * RGBABytes );

   // TODO do on GPU
   /*
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
   */

   return pixels;
}

GPURayTracer::Viewport GPURayTracer::calculateViewport( const TracerOptions& options )
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

void GPURayTracer::addColorToRawPixels( RawPixels& rawPixels, const Color& color, size_t index )
{
   // TODO tone map
   rawPixels[ index ] = toneMapToUint8( color.R );
   rawPixels[ index + 1 ] = toneMapToUint8( color.G );
   rawPixels[ index + 2 ] = toneMapToUint8( color.B );
   rawPixels[ index + 3 ] = color.alpha;
}

uint8_t GPURayTracer::toneMapToUint8( float value )
{
   // Can also use gamma correction, but that seems to bright
   // TODO magical number for exposure fix
   return static_cast<uint8_t>( std::lround( Math::gammaCorrection( Math::exposureToneMapping( value, 1.1f ), 1.6f ) * 255.0f ) );
}
