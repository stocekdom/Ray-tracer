//
// Created by dominik on 21.02.26.
//

#include "Objects.cuh"
#include "Math.cuh"

Ray::Ray( const float4& start, const float4& direction )
   : startPoint( start ), direction( direction ),
     inverseDirection( float4{ 1 / direction.x, 1 / direction.y, 1 / direction.z, 0.f } )
{
}

Light::Light( const float4& center, const Color& color, float intensity )
{
   this->centerPosition = center;
   this->lightColor = color;
   this->intensity = powf( 10.f, intensity );
}

GPU_DEV bool SceneObject::intersects( const Ray& ray, RayHitResult& result ) const
{
   switch( type )
   {
      case SPHERE: return intersectSphere( ray, result );
      case PLANE: return intersectPlane( ray, result );
      case BLOCK: return intersectBlock( ray, result );
      default:
         return false;
   }
}

// Math behind ray-sphere intersection: https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-sphere-intersection.html
GPU_DEV bool SceneObject::intersectSphere( const Ray& ray, RayHitResult& result ) const
{
   const auto offsetCenter = ray.startPoint - centerPosition;
   // Optional check. However, branching costs something so we ignore it and only construct normalized rays in the Ray-Tracer class
   /*
   if( std::hypot( ray.direction.x, ray.direction.y, ray.direction.z ) != 1.f )
      throw std::runtime_error( "Ray direction is not normalized" );
   */

   // We can ignore 'a' since it is equal to Direction^2, which is a dot product of Direction vector.
   // However, the direction vector is normalized, so the result is 1, and it won't play a part in the quadratic formula solutions
   const float b = dotProduct3( ray.direction, offsetCenter );
   const float c = dotProduct3( offsetCenter, offsetCenter ) - ( data.sphere.radius * data.sphere.radius );
   float discriminant = ( b * b ) - c;

   // No real solution
   if( discriminant < FLT_EPSILON )
      return false;

   discriminant = sqrtf( discriminant );
   float root = -b - discriminant;

   if( root < DISTANCE_EPSILON )
   {
      root = -b + discriminant;
      if( root < DISTANCE_EPSILON )
         return false;
   }

   result.distance = root;
   // This math is moved into a ray tracer to optimize instruction count. Normals can be moved to if we switch to enum objects
   result.hitPoint = ray.startPoint + ( result.distance * ray.direction );
   result.normal = result.hitPoint - centerPosition;
   normalize3( result.normal );
   return true;
}

// Math behind plane intersection: https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-plane-and-ray-disk-intersection.html
GPU_DEV bool SceneObject::intersectPlane( const Ray& ray, RayHitResult& result ) const
{
   auto denominator = dotProduct3( data.plane.normal, ray.direction );

   // Check if we don't divide by 0. If the denominator is 0 the plane and the ray are parallel and never intersect
   if( fabsf( denominator ) < FLT_EPSILON )
      return false;

   float distance = dotProduct3( centerPosition - ray.startPoint, data.plane.normal ) / denominator;

   if( distance < DISTANCE_EPSILON )
      return false;

   // TODO add a check for planes dimensions to make the intersection work with finite planes

   result.normal = data.plane.normal;
   result.distance = distance;
   result.hitPoint = ray.startPoint + ( result.distance * ray.direction );
   return true;
}

GPU_DEV bool SceneObject::intersectBlock( const Ray& ray, RayHitResult& result ) const
{
   // Slab method
   float4 t1 = hadamardProduct3( data.block.minPoint - ray.startPoint, ray.inverseDirection );
   float4 t2 = hadamardProduct3( data.block.maxPoint - ray.startPoint, ray.inverseDirection );

   float4 tSmaller = minf4( t1, t2 );
   float4 tBigger = maxf4( t1, t2 );

   float tMin = fmaxf( fmaxf( tSmaller.x, tSmaller.y ), tSmaller.z );
   float tMax = fminf( fminf( tBigger.x, tBigger.y ), tBigger.z );

   if( tMax < fmaxf( tMin, 0.f ) )
      return false;

   // Check if the ray starts inside the block. If it does hit point is the tMax * ray.direction instead of tMin
   bool isInside = tMin < 0.f;
   result.distance = isInside ? tMax : tMin;
   result.hitPoint = ray.startPoint + ( result.distance * ray.direction );

   // Calculate which block side got hit to get the normal. Warp divergence is minimal since there's a very low change a ray will be inside the block
   int axis = 0;

   if( !isInside )
   {
      axis = ( tSmaller.y > tSmaller.x ) ? 1 : 0;
      axis = ( tSmaller.z > get( tSmaller, axis ) ) ? 2 : axis;
   }
   else
   {
      axis = ( tBigger.y < tBigger.x ) ? 1 : 0;
      axis = ( tBigger.z < get( tBigger, axis ) ) ? 2 : axis;
   }

   result.normal = float4{ 0.f, 0.f, 0.f, 0.f };
   set( result.normal, axis, copysignf( 1.0f, get( -ray.direction, axis ) ) );

   if( isInside )
      result.normal = -result.normal;

   return true;
}
