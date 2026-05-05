//
// Created by dominik on 18.02.26.
//

#ifndef SEQUENCIAL_OBJECTS_H
#define SEQUENCIAL_OBJECTS_H

#include "Color.cuh"
#include "Material.h"
#include "RayTraceResult.h"
#include "Vector.cuh"

// Using single precision (float). If needed, switch to Vector3d for double precision

struct Ray
{
   Vector3f startPoint;
   // This should be normalized
   Vector3f direction;
};

class Light
{
   public:
      Light( const Vector3f& center, const Color& color, float intensity );

      Vector3f centerPosition;
      Color lightColor;
      float intensity;
};

enum class ObjectType : uint8_t
{
   SPHERE,
   PLANE,
   BLOCK
};

struct SphereData
{
   float radius;
};

struct PlaneData
{
   Vector3f normal;
   float halfWidth, halfDepth;
};

struct BlockData
{
   Vector3f extents;
};

struct SceneObject
{
   Vector3f centerPosition;
   Material material;
   ObjectType type;

   union ObjectData
   {
      SphereData sphere;
      PlaneData plane;
      BlockData block;
   } data;

   static constexpr float DISTANCE_EPSILON = 0.0001f;

   GPU_HOST static SceneObject makeSphere( const Vector3f& center, const Material& material, float radius )
   {
      ObjectData data{};
      data.sphere = SphereData{ radius };
      return SceneObject{ center, material, ObjectType::PLANE, data };
   }

   static SceneObject makePlane( const Vector3f& center, const Material& material,
                                 const Vector3f& normal, float halfWidth, float halfDepth )
   {
      ObjectData data{};
      data.plane = PlaneData{ normal, halfWidth, halfDepth };
      return SceneObject{ center, material, ObjectType::PLANE, data };
   }

   static SceneObject makeBlock( const Vector3f& center, const Material& material, const Vector3f& extents )
   {
      ObjectData data{};
      data.block = BlockData{ extents };
      return SceneObject{ center, material, ObjectType::PLANE, data };
   }

   /**
    * @brief Determines if an object intersects a ray.
    *
    * @warning Uses switch to determine the shape and calls the appropriate intersect method -> Can cause warp divergence
    *
    * @param ray The ray to test the intersection with
    * @param result Out parameter that is filled with the intersection data if it occurred. Otherwise, it's left as-is
    * @return True if the ray intersects the object. False if it doesn't
   */
   GPU_DEV bool intersects( const Ray& ray, RayHitResult& result ) const;

   private:
      GPU_DEV bool intersectSphere( const Ray& ray, RayHitResult& result ) const;

      GPU_DEV bool intersectPlane( const Ray& ray, RayHitResult& result ) const;

      GPU_DEV bool intersectBlock( const Ray& ray, RayHitResult& result ) const;
};

#endif //SEQUENCIAL_OBJECTS_H
