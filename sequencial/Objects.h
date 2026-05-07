//
// Created by dominik on 18.02.26.
//

#ifndef SEQUENCIAL_OBJECTS_H
#define SEQUENCIAL_OBJECTS_H

#include "Color.h"
#include "Material.h"
#include "Vector.h"

// Using single precision (float). If needed, switch to Vector3d for double precision

struct Ray
{
   Ray( const Vector3f& startPoint, const Vector3f& direction )
      : startPoint( startPoint ), direction( direction ),
        inverseDirection( Vector3f( 1.f / direction.x(), 1.f / direction.y(), 1.f / direction.z() ) )
   {
   }

   Vector3f startPoint;
   // This should be normalized
   Vector3f direction;
   // Inverse of the direction for faster block intersection calculation
   // WARN has to change if direction changes. Currently is public for easier access
   // TODO setters
   Vector3f inverseDirection;
};

class Light
{
   public:
      Light( const Vector3f& center, const Color& color, float intensity );

      Vector3f centerPosition;
      Color lightColor;
      float intensity;
};

// TODO For CUDA use an enum instead of virtual method

struct RayHitResult
{
   float distance = std::numeric_limits<float>::infinity();
   Vector3f hitPoint;
   Vector3f normal;
};

class SceneObject
{
   public:
      SceneObject() = delete;

      SceneObject( const Vector3f& center, const Material& material );

      virtual ~SceneObject() = default;

      /**
       * @brief Determines if an object intersects a ray.
       *
       * Uses polymorphism, so each object can determine its own intersect method
       *
       * @param ray
       * @param result Out parameter which is filled with the intersection data if it occurred. Otherwise, it's left as-is
       * @return True if the ray intersects the object. False if it doesn't
       */
      virtual bool intersects( const Ray& ray, RayHitResult& result ) const = 0;

      Vector3f centerPosition;
      Material material{};
      static constexpr float EPSILON = 0.0001f;
};

class Sphere : public SceneObject
{
   public:
      Sphere() = delete;

      Sphere( const Vector3f& center, const Material& material, float radius );

      bool intersects( const Ray& ray, RayHitResult& result ) const override;

      float radius{};
};

class Plane : public SceneObject
{
   public:
      Plane() = delete;

      Plane( const Vector3f& center, const Material& material, const Vector3f& normal, float halfWidth, float halfDepth );

      bool intersects( const Ray& ray, RayHitResult& result ) const override;

      Vector3f normal;
      // These values represent the plane dimensions so that we don't have infinite planes.
      // From the center point we define half-width and half-depth to limit the plane
      float halfWidth{};
      float halfDepth{};
};

// Axis alligned box for now
class Block : public SceneObject
{
   public:
      Block() = delete;

      Block( const Vector3f& center, const Material& material, const Vector3f& extents );

      bool intersects( const Ray& ray, RayHitResult& result ) const override;

      Vector3f minPoint;
      Vector3f maxPoint;
};

#endif //SEQUENCIAL_OBJECTS_H
