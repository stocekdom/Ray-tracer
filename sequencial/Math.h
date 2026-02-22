//
// Created by dominik on 21.02.26.
//

#ifndef SEQUENCIAL_MATH_H
#define SEQUENCIAL_MATH_H

#include "Vector.h"
#include <cstdint>

namespace Math
{
   template<typename T>
   T dotProduct( const Vector3<T>& lhs, const Vector3<T>& rhs )
   {
      return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
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

#endif //SEQUENCIAL_MATH_H
