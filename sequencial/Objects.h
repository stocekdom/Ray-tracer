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
   Vector3f startPoint;
   // This should be normalized
   Vector3f direction;
};

struct Light
{
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
      SceneObject() = default;

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
      Sphere() = default;

      Sphere( const Vector3f& center, const Material& material, float radius );

      bool intersects( const Ray& ray, RayHitResult& result ) const override;

      float radius{};
};

struct Plane : SceneObject
{
   Vector3f normal;
};

struct Block : SceneObject
{
   Vector3f extents;
};

#endif //SEQUENCIAL_OBJECTS_H
