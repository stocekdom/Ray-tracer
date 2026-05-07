//
// Created by dominik on 18.02.26.
//

#ifndef GPURAYTRACER_VECTOR_H
#define GPURAYTRACER_VECTOR_H
#include <algorithm>
#include <complex>
#include <ostream>

// TODO use native cuda float4
template<typename T, size_t N>
class Vector
{
   public:
      GPU_HD Vector()
      {
         std::fill( members, members + N, T{} );
      }

      template<typename... Args>
      GPU_HD Vector( Args... args ) : members{ static_cast<T>( args )... }
      {
         static_assert( sizeof...( Args ) == N, "Invalid number of arguments for Vector constructor" );
      }

      GPU_HD Vector( const Vector<T, N>& other ) = default;

      GPU_HD Vector( Vector<T, N>&& other ) noexcept = default;

      GPU_HD ~Vector() = default;

      GPU_HD Vector& operator=( const Vector& ) = default;

      GPU_HD Vector& operator=( Vector&& ) noexcept( std::is_nothrow_move_assignable_v<T> ) = default;

      template<typename U>
      GPU_HD explicit Vector( const Vector<U, N>& other )
      {
         for( size_t i = 0; i < N; ++i )
         {
            members[ i ] = static_cast<T>( other[ i ] );
         }
      }

      GPU_HD T& operator[]( size_t index )
      {
         return members[ index ];
      }

      GPU_HD const T& operator[]( size_t index ) const
      {
         return members[ index ];
      }

      // Other methods
      GPU_HD T getEuclideanDistance( const Vector<T, N>& other ) const
      {
         T sum = 0;
         for( size_t i = 0; i < N; ++i )
         {
            T diff = members[ i ] - other.members[ i ];
            sum += diff * diff;
         }
         return std::sqrt( sum );
      }

      GPU_HD void normalize() requires std::floating_point<T>
      {
         T lengthSq = 0;
         for( size_t i = 0; i < N; ++i )
         {
            lengthSq += members[ i ] * members[ i ];
         }
         T length = std::sqrt( lengthSq );

         if( length <= std::numeric_limits<T>::epsilon() )
         {
            std::fill( members, members + N, T{ 0 } );
            return;
         }

         for( size_t i = 0; i < N; ++i )
         {
            members[ i ] /= length;
         }
      }

   protected:
      T members[ N ];
};

template<typename T>
struct Vector3 : Vector<T, 3>
{
   using Vector<T, 3>::Vector;

   GPU_HD Vector3() : Vector<T, 3>()
   {
   }

   GPU_HD Vector3( T x, T y, T z ) : Vector<T, 3>( x, y, z )
   {
   }

   template<typename U>
   GPU_HD Vector3( const Vector<U, 3>& other ) : Vector<T, 3>( other )
   {
   }

   GPU_HD T& x() { return this->members[ 0 ]; }
   GPU_HD T& y() { return this->members[ 1 ]; }
   GPU_HD T& z() { return this->members[ 2 ]; }

   GPU_HD const T& x() const { return this->members[ 0 ]; }
   GPU_HD const T& y() const { return this->members[ 1 ]; }
   GPU_HD const T& z() const { return this->members[ 2 ]; }
};

template<typename T>
struct Vector4 : Vector<T, 4>
{
   using Vector<T, 4>::Vector;

   GPU_HD Vector4() : Vector<T, 4>()
   {
   }

   GPU_HD Vector4( T x_val, T y_val, T z_val, T w_val ) : Vector<T, 4>( x_val, y_val, z_val, w_val )
   {
   }

   GPU_HD Vector4( const Vector<T, 4>& other ) : Vector<T, 4>( other )
   {
   }

   GPU_HD T& x() { return this->members[ 0 ]; }
   GPU_HD T& y() { return this->members[ 1 ]; }
   GPU_HD T& z() { return this->members[ 2 ]; }
   GPU_HD T& w() { return this->members[ 3 ]; }

   GPU_HD const T& x() const { return this->members[ 0 ]; }
   GPU_HD const T& y() const { return this->members[ 1 ]; }
   GPU_HD const T& z() const { return this->members[ 2 ]; }
   GPU_HD const T& w() const { return this->members[ 3 ]; }
};

namespace VectorOps
{
   template<typename T, size_t N>
   GPU_HD T dotProduct( const Vector<T, N>& lhs, const Vector<T, N>& rhs )
   {
      T result = 0;
      for( size_t i = 0; i < N; ++i )
      {
         result += lhs[ i ] * rhs[ i ];
      }
      return result;
   }

   template<typename T, size_t N>
   GPU_HD Vector<T, N> hadamardProduct( const Vector<T, N>& lhs, const Vector<T, N>& rhs )
   {
      Vector<T, N> result;
      for( size_t i = 0; i < N; ++i )
      {
         result[ i ] = lhs[ i ] * rhs[ i ];
      }
      return result;
   }

   // Constructs a new vector with pairwise minimums from lhs and rhs
   template<typename T, size_t N>
   GPU_HD Vector<T, N> min( const Vector<T, N>& lhs, const Vector<T, N>& rhs )
   {
      Vector<T, N> result;
      for( size_t i = 0; i < N; ++i )
      {
         result[ i ] = std::min( lhs[ i ], rhs[ i ] );
      }
      return result;
   }

   // Constructs a new vector with pairwise maximums from lhs and rhs
   template<typename T, size_t N>
   GPU_HD Vector<T, N> max( const Vector<T, N>& lhs, const Vector<T, N>& rhs )
   {
      Vector<T, N> result;
      for( size_t i = 0; i < N; ++i )
      {
         result[ i ] = std::max( lhs[ i ], rhs[ i ] );
      }
      return result;
   }
}

template<typename T, size_t N>
HOST_DEV std::ostream& operator<<( std::ostream& os, const Vector<T, N>& v )
{
   os << "(";
   for( size_t i = 0; i < N; ++i )
   {
      os << v[ i ] << ( i == N - 1 ? "" : ", " );
   }
   os << ")";
   return os;
}

template<typename T, size_t N>
GPU_HD Vector<T, N> operator+( const Vector<T, N>& lhs, const Vector<T, N>& rhs )
{
   Vector<T, N> result;
   for( size_t i = 0; i < N; ++i )
   {
      result[ i ] = lhs[ i ] + rhs[ i ];
   }
   return result;
}

template<typename T, size_t N>
GPU_HD Vector<T, N>& operator+=( Vector<T, N>& lhs, const Vector<T, N>& rhs )
{
   for( size_t i = 0; i < N; ++i )
   {
      lhs[ i ] += rhs[ i ];
   }
   return lhs;
}

template<typename T, size_t N>
GPU_HD Vector<T, N> operator-( const Vector<T, N>& v )
{
   Vector<T, N> result;
   for( size_t i = 0; i < N; ++i )
   {
      result[ i ] = -v[ i ];
   }
   return result;
}

template<typename T, size_t N>
GPU_HD Vector<T, N> operator-( const Vector<T, N>& lhs, const Vector<T, N>& rhs )
{
   Vector<T, N> result;
   for( size_t i = 0; i < N; ++i )
   {
      result[ i ] = lhs[ i ] - rhs[ i ];
   }
   return result;
}

template<typename T, size_t N>
GPU_HD Vector<T, N>& operator-=( Vector<T, N>& lhs, const Vector<T, N>& rhs )
{
   for( size_t i = 0; i < N; ++i )
   {
      lhs[ i ] -= rhs[ i ];
   }
   return lhs;
}

template<typename T, size_t N>
GPU_HD Vector<T, N> operator*( const Vector<T, N>& lhs, const T& rhs )
{
   Vector<T, N> result;
   for( size_t i = 0; i < N; ++i )
   {
      result[ i ] = lhs[ i ] * rhs;
   }
   return result;
}

template<typename T, size_t N>
GPU_HD Vector<T, N> operator*( const T& lhs, const Vector<T, N>& rhs )
{
   Vector<T, N> result;
   for( size_t i = 0; i < N; ++i )
   {
      result[ i ] = lhs * rhs[ i ];
   }
   return result;
}

template<typename T, size_t N>
GPU_HD Vector<T, N>& operator*=( Vector<T, N>& lhs, const T& rhs )
{
   for( size_t i = 0; i < N; ++i )
   {
      lhs[ i ] *= rhs;
   }
   return lhs;
}

template<typename T, size_t N>
GPU_HD Vector<T, N> operator/( const Vector<T, N>& lhs, const T& rhs )
{
   Vector<T, N> result;
   for( size_t i = 0; i < N; ++i )
   {
      result[ i ] = lhs[ i ] / rhs;
   }
   return result;
}

template<typename T, size_t N>
GPU_HD Vector<T, N>& operator/=( Vector<T, N>& lhs, const T& rhs )
{
   for( size_t i = 0; i < N; ++i )
   {
      lhs[ i ] /= rhs;
   }
   return lhs;
}

template<typename T, size_t N>
GPU_HD bool operator==( const Vector<T, N>& lhs, const Vector<T, N>& rhs )
{
   for( size_t i = 0; i < N; ++i )
   {
      if( lhs[ i ] != rhs[ i ] ) return false;
   }
   return true;
}

template<typename T, size_t N>
GPU_HD bool operator!=( const Vector<T, N>& lhs, const Vector<T, N>& rhs )
{
   return !( lhs == rhs );
}

typedef Vector3<double> Vector3d;
typedef Vector3<float> Vector3f;
typedef Vector3<int> Vector3i;

typedef Vector4<double> Vector4d;
typedef Vector4<float> Vector4f;
typedef Vector4<int> Vector4i;

#endif //GPURAYTRACER_VECTOR_H
