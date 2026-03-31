//
// Created by dominik on 20.02.26.
//
#include "Color.h"
#include "Math.h"

Color::Color() : R( 0.f ), G( 0.f ), B( 0.f ), alpha( 0 )
{
}

Color::Color( float R, float G, float B, uint8_t alpha ) : R( R ), G( G ), B( B ), alpha( alpha )
{
}

std::ostream& operator<<( std::ostream& os, const Color& v )
{
   os << "(" << v.R << ", " << v.G << ", " << v.B << ", " << v.alpha << ")";
   return os;
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
   c1.R += c2.R;
   c1.G += c2.G;
   c1.B += c2.B;
   return c1;
}

Color operator-( const Color& c1, const Color& c2 )
{
   Color c3( c1 );
   return c3 -= c2;
}

Color& operator-=( Color& c1, const Color& c2 )
{
   c1.R -= c2.R;
   c1.G -= c2.G;
   c1.B -=c2.B;
   return c1;
}

Color operator*( const Color& c1, const Color& c2 )
{
   Color c3( c1.R * c2.R, c1.G * c2.G, c1.B * c2.B );
   return c3;
}

Color operator*( const Color& c1, float f )
{
   return { c1.R * f, c1.G * f, c1.B * f, c1.alpha };
}

Color& operator*=( Color& c1, float f )
{
   c1.R *= f;
   c1.G *= f;
   c1.B *= f;
   return c1;
}
