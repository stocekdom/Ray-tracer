//
// Created by dominik on 20.02.26.
//
#include "Color.h"
#include "Math.h"

Color::Color() : R( 0 ), G( 0 ), B( 0 ), alpha( 0 )
{
}

Color::Color( uint8_t R, uint8_t G, uint8_t B, uint8_t alpha ) : R( R ), G( G ), B( B ), alpha( alpha )
{
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
   c1.R = Math::uint8ClampAddition( c1.R, c2.R );
   c1.G = Math::uint8ClampAddition( c1.G, c2.G );
   c1.B = Math::uint8ClampAddition( c1.B, c2.B );
   return c1;
}

Color operator-( const Color& c1, const Color& c2 )
{
   Color c3( c1 );
   return c3 -= c2;
}

Color& operator-=( Color& c1, const Color& c2 )
{
   c1.R = Math::uint8ClampSubtraction( c1.R, c2.R );
   c1.G = Math::uint8ClampSubtraction( c1.G, c2.G );
   c1.B = Math::uint8ClampSubtraction( c1.B, c2.B );
   return c1;
}

Color operator*( const Color& c1, float f )
{
   return {
      Math::uint8ClampMultiplication( c1.R, f ),
      Math::uint8ClampMultiplication( c1.G, f ),
      Math::uint8ClampMultiplication( c1.B, f ),
      c1.alpha
   };
}

Color& operator*=( Color& c1, float f )
{
   c1.R = Math::uint8ClampMultiplication( c1.R, f );
   c1.G = Math::uint8ClampMultiplication( c1.G, f );
   c1.B = Math::uint8ClampMultiplication( c1.B, f );
   return c1;
}
