//
// Created by dominik on 18.02.26.
//

#ifndef SEQUENCIAL_OBJECTS_H
#define SEQUENCIAL_OBJECTS_H

#include "Color.h"
#include "Vector.h"

// Using single precision (float). If needed switch to Vector3d for double precision

struct Object
{
   Vector3f centerPosition;
};

struct Sphere : Object
{
   double radius{};
};

struct Plane : Object
{
   Vector3f normal;
};

struct Block : Object
{
   Vector3f extents;
};

struct Ray : Object
{
   // This should be normalized
   Vector3f direction;
};

struct Light
{
   Vector3f centerPosition;
   Color lightColor;
   float intensity;
};

#endif //SEQUENCIAL_OBJECTS_H
