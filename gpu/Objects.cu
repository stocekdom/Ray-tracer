//
// Created by dominik on 21.02.26.
//

#include "Objects.cuh"
#include "Math.cuh"
#include <stdexcept>

Light::Light( const Vector3f& center, const Color& color, float intensity )
{
   this->centerPosition = center;
   this->lightColor = color;
   this->intensity = std::pow( 10.f, intensity );
}

bool SceneObject::intersects( const Ray& ray, RayHitResult& result ) const
{
   switch( type )
   {
      case ObjectType::SPHERE: return intersectSphere( ray, result );
      case ObjectType::PLANE: return intersectPlane( ray, result );
      case ObjectType::BLOCK: return intersectBlock( ray, result );
      default:
         throw std::runtime_error( "Invalid object type in intersects method call" );
   }
}

// Math behind ray-sphere intersection: https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-sphere-intersection.html
bool SceneObject::intersectSphere( const Ray& ray, RayHitResult& result ) const
{
   const auto offsetCenter = ray.startPoint - centerPosition;
   // Optional check. However, branching costs something so we ignore it and only construct normalized rays in the Ray-Tracer class
   /*
   if( std::hypot( ray.direction.x, ray.direction.y, ray.direction.z ) != 1.f )
      throw std::runtime_error( "Ray direction is not normalized" );
   */

   // We can ignore 'a' since it is equal to Direction^2, which is a dot product of Direction vector.
   // However, the direction vector is normalized, so the result is 1, and it won't play a part in the quadratic formula solutions
   const float b = dotProduct( ray.direction, offsetCenter );
   const float c = dotProduct( offsetCenter, offsetCenter ) - ( data.sphere.radius * data.sphere.radius );
   float discriminant = ( b * b ) - c;

   // No real solution
   if( discriminant < Math::EPSILON )
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
   result.normal.normalize();
   return true;
}

// Math behind plane intersection: https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-plane-and-ray-disk-intersection.html
bool SceneObject::intersectPlane( const Ray& ray, RayHitResult& result ) const
{
   auto denominator = dotProduct( data.plane.normal, ray.direction );

   // Check if we don't divide by 0. If the denominator is 0 the plane and the ray are parallel and never intersect
   if( fabsf( denominator ) < Math::EPSILON )
      return false;

   float distance = dotProduct( centerPosition - ray.startPoint, data.plane.normal ) / denominator;

   if( distance < DISTANCE_EPSILON )
      return false;

   // TODO add a check for planes dimensions to make the intersection work with finite planes

   result.normal = data.plane.normal;
   result.distance = distance;
   result.hitPoint = ray.startPoint + ( result.distance * ray.direction );
   return true;
}

bool SceneObject::intersectBlock( const Ray& ray, RayHitResult& result ) const
{
   throw std::runtime_error( "Block intersection not implemented" );
}
