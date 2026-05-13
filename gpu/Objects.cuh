//
// Created by dominik on 18.02.26.
//

#ifndef GPURAYTRACER_OBJECTS_H
#define GPURAYTRACER_OBJECTS_H

#include <cstdint>
#include "Color.cuh"
#include "Material.h"
#include "RayTraceResult.h"
#include "Vector.cuh"

// Using single precision (float). If needed, switch to double for double precision

struct Ray
{
   GPU_HD Ray( const float4& start, const float4& direction );

   float4 startPoint;
   // This should be normalized
   float4 direction;
   // Inverse of the direction for faster block intersection calculation
   // WARN has to change if direction changes. Currently is public for easier access
   // TODO setters
   float4 inverseDirection;
};

class Light
{
   public:
      Light( const float4& center, const Color& color, float intensity );

      float4 centerPosition{};
      Color lightColor;
      float intensity;
};

enum ObjectType : uint8_t
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
   float4 normal;
   float halfWidth, halfDepth;
};

struct BlockData
{
   float4 minPoint;
   float4 maxPoint;
};

struct SceneObject
{
   float4 centerPosition;
   Material material;
   ObjectType type;

   union ObjectData
   {
      SphereData sphere;
      PlaneData plane;
      BlockData block;
   } data;

   static constexpr float DISTANCE_EPSILON = 0.0001f;

   HOST_DEV static SceneObject makeSphere( const float4& center, const Material& material, float radius )
   {
      ObjectData data{};
      data.sphere = SphereData{ radius };
      return SceneObject{ center, material, SPHERE, data };
   }

   HOST_DEV static SceneObject makePlane( const float4& center, const Material& material,
                                          const float4& normal, float halfWidth, float halfDepth )
   {
      ObjectData data{};
      data.plane = PlaneData{ normal, halfWidth, halfDepth };
      return SceneObject{ center, material, PLANE, data };
   }

   HOST_DEV static SceneObject makeBlock( const float4& center, const Material& material, const float4& extents )
   {
      ObjectData data{};
      data.block = BlockData{ center - extents, center + extents };
      return SceneObject{ center, material, BLOCK, data };
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

#endif //GPURAYTRACER_OBJECTS_H
