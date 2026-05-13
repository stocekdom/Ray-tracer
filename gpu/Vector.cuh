//
// Created by dominik on 18.02.26.
//

#ifndef GPURAYTRACER_VECTOR_H
#define GPURAYTRACER_VECTOR_H
#include "CUDAAnnotations.cuh"
#include <cmath>
#include <cfloat>
#include <ostream>

GPU_HD inline float get( float4 data, size_t index )
{
   // Float4 is packed and we can index individual elements
   return reinterpret_cast<float*>( &data )[ index ];
}

GPU_HD inline void set( float4& data, size_t index, float value )
{
   reinterpret_cast<float*>( &data )[ index ] = value;
}

GPU_HD inline float safeGet( float4 data, size_t index )
{
   // TODO indicate error
   if( index >= 4 )
      return -1;

   return get( data, index );
}

/**
 * Calculates the euclidian distance of two float4 vectors in 3D
 * Only uses the x,y,z elements to calculate the distance
 * @param lhs
 * @param rhs
 * @return The euclidian distance of two vectors
 */
GPU_HD inline float euclideanDistance3( const float4& lhs, const float4& rhs )
{
   return sqrtf(
      ( lhs.x - rhs.x ) * ( lhs.x - rhs.x )
      + ( lhs.y - rhs.y ) * ( lhs.y - rhs.y )
      + ( lhs.z - rhs.z ) * ( lhs.z - rhs.z ) );
}

GPU_HD inline float euclideanDistance( const float4& lhs, const float4& rhs )
{
   return sqrtf(
      ( lhs.x - rhs.x ) * ( lhs.x - rhs.x )
      + ( lhs.y - rhs.y ) * ( lhs.y - rhs.y )
      + ( lhs.z - rhs.z ) * ( lhs.z - rhs.z )
      + ( lhs.w - rhs.w ) * ( lhs.w - rhs.w ) );
}

GPU_HD inline void normalize3( float4& data )
{
   float length = data.x * data.x + data.y * data.y + data.z * data.z;
   length = sqrtf( length );

   if( length <= FLT_EPSILON )
   {
      data = float4{ 0, 0, 0, 0 };
      return;
   }

   data = float4{ data.x / length, data.y / length, data.z / length, 0 };
}

GPU_HD inline float4 minf4( const float4& lhs, const float4& rhs )
{
   return float4{ fminf( lhs.x, rhs.x ), fminf( lhs.y, rhs.y ), fminf( lhs.z, rhs.z ), fminf( lhs.w, rhs.w ) };
}

GPU_HD inline float4 maxf4( const float4& lhs, const float4& rhs )
{
   return float4{ fmaxf( lhs.x, rhs.x ), fmaxf( lhs.y, rhs.y ), fmaxf( lhs.z, rhs.z ), fmaxf( lhs.w, rhs.w ) };
}

GPU_HD inline float dotProduct3( const float4& lhs, const float4& rhs )
{
   return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}

GPU_HD inline float4 hadamardProduct3( const float4& lhs, const float4& rhs )
{
   return float4{ lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z, 0.f };
}

HOST_DEV inline std::ostream& operator<<( std::ostream& os, const float4& rhs )
{
   os << "(";
   os << rhs.x << ", " << rhs.y << ", " << rhs.z << ", " << rhs.w;
   os << ")";
   return os;
}

GPU_HD inline float4 operator+( const float4& lhs, const float4& rhs )
{
   return float4{ lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w };
}

GPU_HD inline float4 operator-( const float4& rhs )
{
   return float4{ -rhs.x, -rhs.y, -rhs.z, -rhs.w };
}

GPU_HD inline float4 operator-( const float4& lhs, const float4& rhs )
{
   return float4{ lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w };
}

GPU_HD inline float4 operator*( const float4& lhs, float rhs )
{
   return float4{ lhs.x * rhs, lhs.y * rhs, lhs.z * rhs, lhs.w * rhs };
}

GPU_HD inline float4 operator*( float lhs, const float4& rhs )
{
   return float4{ lhs * rhs.x, lhs * rhs.y, lhs * rhs.z, lhs * rhs.w };
}

GPU_HD inline float4 operator/( const float4& lhs, float rhs )
{
   return float4{ lhs.x / rhs, lhs.y / rhs, lhs.z / rhs, lhs.w / rhs };
}

GPU_HD inline bool operator==( const float4& lhs, const float4& rhs )
{
   return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z && lhs.w == rhs.w;
}

GPU_HD inline bool operator!=( const float4& lhs, const float4& rhs )
{
   return !( lhs == rhs );
}

#endif //GPURAYTRACER_VECTOR_H
