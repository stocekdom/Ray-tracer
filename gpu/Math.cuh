//
// Created by dominik on 21.02.26.
//
#ifndef GPURAYTRACER_MATH_H
#define GPURAYTRACER_MATH_H

#include "CUDAAnnotations.h"
#include <algorithm>
#include <cstdint>
#include <cmath>

// Uses ifdefs for usage on host and on the device since both use different functions
namespace Math
{
   static constexpr float EPSILON = 1e-6;

   GPU_HD [[nodiscard]] inline float saturate( float value )
   {
#ifdef __CUDA_ARCH__
      return __saturatef( value );
#else
      return std::clamp( value, 0.0f, 1.0f );
#endif
   }

   GPU_HD [[nodiscard]] inline float gammaCorrection( float value, float gamma )
   {
#ifdef __CUDA_ARCH__
      return powf( value, 1.0f / gamma );
#else
      return std::pow( value, 1.0f / gamma );
#endif
   }

   GPU_HD [[nodiscard]] inline float reinhardToneMapping( float value )
   {
      return value / ( 1.0f + value );
   };

   GPU_HD [[nodiscard]] inline float exposureToneMapping( float value, float exposure )
   {
#ifdef __CUDA_ARCH__
      return 1.0f - expf( -value * exposure );
#else
      return 1.0f - std::exp( -value * exposure );
#endif
   }

   GPU_HD [[nodiscard]] inline float acesFilmicToneMapping( float value )
   {
      constexpr float a = 2.51f, b = 0.03f, c = 2.43f, d = 0.59f, e = 0.14f;
      return saturate( ( value * ( a * value + b ) ) / ( value * ( c * value + d ) + e ) );
   }

   GPU_HD [[nodiscard]] inline float lerp( float a, float b, float factor )
   {
#ifdef __CUDA_ARCH__
      // Lerp formula using fused multiply add for fewer instructions
      return fma( factor, b, fma( -factor, a, a ) )
#else
      return a + factor * ( b - a );
#endif
   }

   [[nodiscard]] inline uint8_t uint8ClampMultiplication( uint8_t value, float factor )
   {
      /*
      float f = std::round( x * factor ); // or std::lround
      f = std::clamp( f, 0.0f, 255.0f );
      return static_cast<uint8_t>( f );
      */

      // Version only using int clamping
      int v = static_cast<int>( std::lround( static_cast<float>( value ) * factor ) );
      if( v < 0 ) v = 0;
      else if( v > 255 ) v = 255;
      return static_cast<uint8_t>( v );
   }

   [[nodiscard]] inline uint8_t uint8ClampAddition( uint8_t value, uint8_t addend )
   {
      unsigned int v = static_cast<unsigned int>( value ) + static_cast<unsigned int>( addend );
      if( v > 255 ) v = 255;
      return static_cast<uint8_t>( v );
   }

   [[nodiscard]] inline uint8_t uint8ClampSubtraction( uint8_t value, uint8_t subtrahend )
   {
      int v = static_cast<int>( value ) - static_cast<int>( subtrahend );
      if( v < 0 ) v = 0;
      return static_cast<uint8_t>( v );
   }
}

#endif //GPURAYTRACER_MATH_H
