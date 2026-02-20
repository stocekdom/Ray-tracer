//
// Created by dominik on 20.02.26.
//
#include "Color.h"

#include <cmath>

Color::Color() : R( 0 ), G( 0 ), B( 0 ), alpha( 0 )
{
}

Color::Color( uint8_t R, uint8_t G, uint8_t B, uint8_t alpha ) : R( R ), G( G ), B( B ), alpha( alpha )
{
}

// Anonymous namespace to ensure internal linkage, so no external files can see this helper function
namespace
{
   [[nodiscard]] uint8_t uint8ClampMultiplication( uint8_t value, float factor )
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

   [[nodiscard]] uint8_t uint8ClampAddition( uint8_t value, uint8_t addend )
   {
      unsigned int v = static_cast<unsigned int>( value ) + static_cast<unsigned int>( addend );
      if( v > 255 ) v = 255;
      return static_cast<uint8_t>( v );
   }

   [[nodiscard]] uint8_t uint8ClampSubtraction( uint8_t value, uint8_t subtrahend )
   {
      int v = static_cast<int>( value ) - static_cast<int>( subtrahend );
      if( v < 0 ) v = 0;
      return static_cast<uint8_t>( v );
   }
}

bool operator==( const Color& c1, const Color& c2 )
{
   return c1.R == c2.R && c1.G == c2.G && c1.B == c2.B && c1.alpha == c2.alpha;
}

Color operator+( const Color& c1, const Color& c2 )
{
   Color c3( c1 );
   return c3 += c2;
}

Color& operator+=( Color& c1, const Color& c2 )
{
   c1.R = uint8ClampAddition( c1.R, c2.R );
   c1.G = uint8ClampAddition( c1.G, c2.G );
   c1.B = uint8ClampAddition( c1.B, c2.B );
   return c1;
}

Color operator-( const Color& c1, const Color& c2 )
{
   Color c3( c1 );
   return c3 -= c2;
}

Color& operator-=( Color& c1, const Color& c2 )
{
   c1.R = uint8ClampSubtraction( c1.R, c2.R );
   c1.G = uint8ClampSubtraction( c1.G, c2.G );
   c1.B = uint8ClampSubtraction( c1.B, c2.B );
   return c1;
}

Color operator*( const Color& c1, float f )
{
   return {
      uint8ClampMultiplication( c1.R, f ),
      uint8ClampMultiplication( c1.G, f ),
      uint8ClampMultiplication( c1.B, f ),
      c1.alpha
   };
}

Color& operator*=( Color& c1, float f )
{
   c1.R = uint8ClampMultiplication( c1.R, f );
   c1.G = uint8ClampMultiplication( c1.G, f );
   c1.B = uint8ClampMultiplication( c1.B, f );
   return c1;
}
