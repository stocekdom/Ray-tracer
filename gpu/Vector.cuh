//
// Created by dominik on 18.02.26.
//

#ifndef SEQUENCIAL_VECTOR_H
#define SEQUENCIAL_VECTOR_H

#include "Math.cuh"
#include "CUDAAnnotations.h"

template<typename T>
class Vector3
{
   public:
      T x;
      T y;
      T z;

      GPU_HD Vector3() : x( 0 ), y( 0 ), z( 0 )
      {
      }

      GPU_HD Vector3( T x, T y, T z ) : x( x ), y( y ), z( z )
      {
      }

      GPU_HD Vector3( T value ) : x( value ), y( value ), z( value )
      {
      }

      GPU_HD ~Vector3() = default;

      GPU_HD Vector3( const Vector3<T>& other ) = default;

      GPU_HD Vector3( Vector3<T>&& other ) noexcept = default;

      GPU_HD Vector3& operator=( const Vector3& ) = default;

      GPU_HD Vector3& operator=( Vector3&& ) noexcept( std::is_nothrow_move_assignable_v<T> ) = default;

      template<typename U>
      GPU_HD explicit Vector3( const Vector3<U>& other )
         : x( static_cast<T>( other.x ) ),
           y( static_cast<T>( other.y ) ),
           z( static_cast<T>( other.z ) )
      {
      }

      // Other methods
      GPU_HD T getEuclideanDistance( const Vector3<T>& other ) const
      {
         return sqrt( ( x - other.x ) * ( x - other.x ) + ( y - other.y ) * ( y - other.y ) + ( z - other.z ) * ( z - other.z ) );
      }

      void normalize()
      {
         auto length = sqrt( x * x + y * y + z * z );

         if( length <= static_cast<T>( Math::EPSILON ) )
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
GPU_HD Vector3<T> operator+( const Vector3<T>& lhs, const Vector3<T>& rhs )
{
   return Vector3<T>( lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z );
}

template<typename T>
GPU_HD Vector3<T>& operator+=( Vector3<T>& lhs, const Vector3<T>& rhs )
{
   lhs.x += rhs.x;
   lhs.y += rhs.y;
   lhs.z += rhs.z;
   return lhs;
}

template<typename T>
GPU_HD Vector3<T> operator-( const Vector3<T>& v )
{
   return Vector3<T>( -v.x, -v.y, -v.z );
}

template<typename T>
GPU_HD Vector3<T> operator-( const Vector3<T>& lhs, const Vector3<T>& rhs )
{
   return Vector3<T>( lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z );
}

template<typename T>
GPU_HD Vector3<T>& operator-=( Vector3<T>& lhs, const Vector3<T>& rhs )
{
   lhs.x -= rhs.x;
   lhs.y -= rhs.y;
   lhs.z -= rhs.z;
   return lhs;
}

template<typename T>
GPU_HD Vector3<T> operator*( const Vector3<T>& lhs, const T& rhs )
{
   return Vector3<T>( lhs.x * rhs, lhs.y * rhs, lhs.z * rhs );
}

template<typename T>
GPU_HD Vector3<T> operator*( const T& lhs, const Vector3<T>& rhs )
{
   return Vector3<T>( lhs * rhs.x, lhs * rhs.y, lhs * rhs.z );
}

template<typename T>
GPU_HD Vector3<T>& operator*=( Vector3<T>& lhs, const T& rhs )
{
   lhs.x *= rhs;
   lhs.y *= rhs;
   lhs.z *= rhs;
   return lhs;
}

template<typename T>
GPU_HD Vector3<T> operator/( const Vector3<T>& lhs, const T& rhs )
{
   return Vector3<T>( lhs.x / rhs, lhs.y / rhs, lhs.z / rhs );
}

template<typename T>
GPU_HD Vector3<T>& operator/=( Vector3<T>& lhs, const T& rhs )
{
   lhs.x /= rhs;
   lhs.y /= rhs;
   lhs.z /= rhs;
   return lhs;
}

template<typename T>
GPU_HD bool operator==( const Vector3<T>& lhs, const Vector3<T>& rhs )
{
   return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
}

template<typename T>
GPU_HD bool operator!=( const Vector3<T>& lhs, const Vector3<T>& rhs )
{
   return lhs.x != rhs.x || lhs.y != rhs.y || lhs.z != rhs.z;
}

typedef Vector3<double> Vector3d;
typedef Vector3<float> Vector3f;
typedef Vector3<int> Vector3i;

template<typename T>
GPU_HD T dotProduct( const Vector3<T>& lhs, const Vector3<T>& rhs )
{
   return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}

#endif //SEQUENCIAL_VECTOR_H
