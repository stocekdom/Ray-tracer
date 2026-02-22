//
// Created by dominik on 18.02.26.
//

#ifndef SEQUENCIAL_VECTOR_H
#define SEQUENCIAL_VECTOR_H
#include <algorithm>
#include <complex>
#include <ostream>

template<typename T>
class Vector3
{
   public:
      Vector3() : x( 0 ), y( 0 ), z( 0 )
      {
      }

      Vector3( T x, T y, T z ) : x( x ), y( y ), z( z )
      {
      }

      Vector3( T value ) : x( value ), y( value ), z( value )
      {
      }

      Vector3( const Vector3<T>& other ) = default;

      Vector3( Vector3<T>&& other ) noexcept = default;

      ~Vector3() = default;

      Vector3& operator=( const Vector3& ) = default;

      Vector3& operator=( Vector3&& ) noexcept( std::is_nothrow_move_assignable_v<T> ) = default;

      template<typename U>
      explicit Vector3( const Vector3<U>& other )
         : x( static_cast<T>( other.x ) ),
           y( static_cast<T>( other.y ) ),
           z( static_cast<T>( other.z ) )
      {
      }

      template<typename U>
      explicit Vector3( Vector3<U>&& other )
         : x( static_cast<T>( std::move( other.x ) ) ),
           y( static_cast<T>( std::move( other.y ) ) ),
           z( static_cast<T>( std::move( other.z ) ) )
      {
      }

      T x;
      T y;
      T z;

      // Other methods
      T getEuclideanDistance( const Vector3<T>& other ) const
      {
         return std::sqrt(
            ( x - other.x ) * ( x - other.x ) + ( y - other.y ) * ( y - other.y ) + ( z - other.z ) * ( z - other.z ) );
      }

      void normalize() requires std::floating_point<T>
      {
         auto length = std::hypot( x, y, z );

         if( length <= std::numeric_limits<T>::epsilon() )
         {
            x = y = z = 0;
            return;
         }

         x /= length;
         y /= length;
         z /= length;
      }
};

template<typename T>
std::ostream& operator<<( std::ostream& os, const Vector3<T>& v )
{
   os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
   return os;
}

template<typename T>
Vector3<T> operator+( const Vector3<T>& lhs, const Vector3<T>& rhs )
{
   return Vector3<T>( lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z );
}

template<typename T>
Vector3<T>& operator+=( Vector3<T>& lhs, const Vector3<T>& rhs )
{
   lhs.x += rhs.x;
   lhs.y += rhs.y;
   lhs.z += rhs.z;
   return lhs;
}

template<typename T>
Vector3<T> operator-( const Vector3<T>& v )
{
   return Vector3<T>( -v.x, -v.y, -v.z );
}

template<typename T>
Vector3<T> operator-( const Vector3<T>& lhs, const Vector3<T>& rhs )
{
   return Vector3<T>( lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z );
}

template<typename T>
Vector3<T>& operator-=( Vector3<T>& lhs, const Vector3<T>& rhs )
{
   lhs.x -= rhs.x;
   lhs.y -= rhs.y;
   lhs.z -= rhs.z;
   return lhs;
}

template<typename T>
Vector3<T> operator*( const Vector3<T>& lhs, const T& rhs )
{
   return Vector3<T>( lhs.x * rhs, lhs.y * rhs, lhs.z * rhs );
}

template<typename T>
Vector3<T> operator*( const T& lhs, const Vector3<T>& rhs )
{
   return Vector3<T>( lhs * rhs.x, lhs * rhs.y, lhs * rhs.z );
}

template<typename T>
Vector3<T>& operator*=( Vector3<T>& lhs, const T& rhs )
{
   lhs.x *= rhs;
   lhs.y *= rhs;
   lhs.z *= rhs;
   return lhs;
}

template<typename T>
Vector3<T> operator/( const Vector3<T>& lhs, const T& rhs )
{
   return Vector3<T>( lhs.x / rhs, lhs.y / rhs, lhs.z / rhs );
}

template<typename T>
Vector3<T>& operator/=( Vector3<T>& lhs, const T& rhs )
{
   lhs.x /= rhs;
   lhs.y /= rhs;
   lhs.z /= rhs;
   return lhs;
}

template<typename T>
bool operator==( const Vector3<T>& lhs, const Vector3<T>& rhs )
{
   return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
}

template<typename T>
bool operator!=( const Vector3<T>& lhs, const Vector3<T>& rhs )
{
   return lhs.x != rhs.x || lhs.y != rhs.y || lhs.z != rhs.z;
}

typedef Vector3<double> Vector3d;
typedef Vector3<float> Vector3f;
typedef Vector3<int> Vector3i;

#endif //SEQUENCIAL_VECTOR_H
