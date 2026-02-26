//
// Created by dominik on 21.02.26.
//

#include "Objects.h"
#include "Math.h"

Light::Light( const Vector3f& center, const Color& color, float intensity )
{
   this->centerPosition = center;
   this->lightColor = color;
   this->intensity = std::pow( 10.f, intensity );
}

SceneObject::SceneObject( const Vector3f& center, const Material& material ) : centerPosition( center ), material( material )
{
}

Sphere::Sphere( const Vector3f& center, const Material& material, float radius ) : SceneObject( center, material ),
                                                                                   radius( radius )
{
}

// Math behind ray-sphere intersection: https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-sphere-intersection.html
bool Sphere::intersects( const Ray& ray, RayHitResult& result ) const
{
   // Actually is Ray.Center - Sphere.Center, but our ray is cast from origin which is always (0,0,0)
   auto offsetCenter = -centerPosition;

   // Optional check. However, branching costs something so we ignore it and only construct normalized rays in the Ray-Tracer class
   /*
   if( std::hypot( ray.direction.x, ray.direction.y, ray.direction.z ) != 1.f )
      throw std::runtime_error( "Ray direction is not normalized" );
   */

   // We can ignore a since it is equal to Direction^2, which is a dot product of Direction vector.
   // However, the direction vector is normalized, so the result is 1, and it won't play a part in the quadratic formula solutions
   float b = Math::dotProduct( ray.direction, offsetCenter );
   float c = Math::dotProduct( offsetCenter, offsetCenter ) - ( radius * radius );
   float discriminant = ( b * b ) - c;

   // No real solution
   if( discriminant < std::numeric_limits<float>::epsilon() )
      return false;

   float discriminantSqrt = std::sqrt( discriminant );
   float root = -b - discriminantSqrt;

   if( root < EPSILON )
   {
      root = -b + discriminantSqrt;

      if( root < EPSILON )
         return false;
   }

   result.distance = root;
   // This math is moved into a ray tracer to optimize instruction count
   /*
   result.hitPoint = ray.startPoint + ( result.distance * ray.direction );
   result.normal = result.hitPoint - centerPosition;
   result.normal.normalize();*/
   return true;
}

Plane::Plane( const Vector3f& center, const Material& material, const Vector3f& normal, float halfWidth, float halfDepth )
   : SceneObject( center, material ), normal( normal ), halfWidth( halfWidth ), halfDepth( halfDepth )
{
}

bool Plane::intersects( const Ray& ray, RayHitResult& result ) const
{
   auto denominator = Math::dotProduct( normal, ray.direction );

   // Check if we don't divide by 0. If the denominator is 0 the plane and the ray are parallel and never intersect
   if( std::abs( denominator ) < std::numeric_limits<float>::epsilon() )
      return false;

   //    float distance = Math::dotProduct( centerPosition - ray.startPoint, normal ) / denominator;
   // ray.startPoint is 0,0,0 and can be omitted
   float distance = Math::dotProduct( centerPosition, normal ) / denominator;

   if( distance < EPSILON )
      return false;

   // TODO add a check for planes dimensions to make the intersection work with finite planes

   //result.normal = normal;
   result.distance = distance;
   //result.hitPoint = ray.startPoint + ( result.distance * ray.direction );
   return true;
}
