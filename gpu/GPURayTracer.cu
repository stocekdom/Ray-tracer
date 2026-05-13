//
// Created by dominik on 20.02.26.
//
#include "GPURayTracer.cuh"
#include "GPUArray.cuh"
#include "Math.cuh"
#include "RayTracerKernel.cuh"

Pixels GPURayTracer::generateImage( const TracerOptions& options, const std::vector<SceneObject>& objects,
                                    const std::vector<Light>& lights )
{
   float clampedFOV = std::clamp( options.fieldOfView, 0.0f, MAX_FOV );
   auto viewport = calculateViewport( {
      options.cameraDistance, clampedFOV, options.maxRecursionDepth, options.imageWidth, options.imageHeight,
      options.backgroundColor, options.ambientLightColor
   } );

   Pixels pixels;
   pixels.resize( options.imageWidth * options.imageHeight );
   GPUArray<Color> gpuColors( options.imageWidth * options.imageHeight );
   // Move (copy) data to the gpu memory
   GPUArray<SceneObject> gpuObjects( objects.size() );
   GPUArray<Light> gpuLights( lights.size() );
   gpuObjects.upload( objects );
   gpuLights.upload( lights );

   launchKernel( gpuObjects.readOnlyView(), gpuLights.readOnlyView(), options, viewport, gpuColors.view() );

   gpuColors.download( pixels.data(), pixels.size() );
   return pixels;
}

RawPixels GPURayTracer::generateRawImage( const TracerOptions& options, const std::vector<SceneObject>& objects,
                                          const std::vector<Light>& lights )
{
   float clampedFOV = std::clamp( options.fieldOfView, 0.0f, MAX_FOV );
   auto viewport = calculateViewport( {
      options.cameraDistance, clampedFOV, options.maxRecursionDepth, options.imageWidth, options.imageHeight,
      options.backgroundColor, options.ambientLightColor
   } );

   RawPixels pixels( options.imageWidth * options.imageHeight * RGBABytes );
   Pixels hostPixels;
   hostPixels.resize( options.imageWidth * options.imageHeight );
   GPUArray<Color> gpuPixels( options.imageWidth * options.imageHeight );

   // Move (copy) data to the gpu memory
   GPUArray<SceneObject> gpuObjects( objects.size() );
   GPUArray<Light> gpuLights( lights.size() );
   gpuObjects.upload( objects );
   gpuLights.upload( lights );

   launchKernel( gpuObjects.readOnlyView(), gpuLights.readOnlyView(), options, viewport, gpuPixels.view() );

   gpuPixels.download( hostPixels.data(), hostPixels.size() );
   for( size_t i = 0; i < hostPixels.size(); ++i )
      addColorToRawPixels( pixels, hostPixels[ i ], i * RGBABytes );

   return pixels;
}

Viewport GPURayTracer::calculateViewport( const TracerOptions& options )
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
   rawPixels[ index ] = toneMapToUint8( color.R );
   rawPixels[ index + 1 ] = toneMapToUint8( color.G );
   rawPixels[ index + 2 ] = toneMapToUint8( color.B );
   rawPixels[ index + 3 ] = 255;//color.alpha; currently we don't use alpha
}

uint8_t GPURayTracer::toneMapToUint8( float value )
{
   // Can also use gamma correction, but that seems to bright
   // TODO magical number for exposure fix
   return static_cast<uint8_t>( std::lround( Math::gammaCorrection( Math::exposureToneMapping( value, 1.1f ), 1.6f ) * 255.0f ) );
}
