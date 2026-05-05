//
// Created by dominik on 20.02.26.
//

#ifndef SEQUENCIAL_MATERIAL_H
#define SEQUENCIAL_MATERIAL_H

#include "Color.cuh"

class Material
{
   public:
      Material();

      Material( const Color& baseColor, float specular, float diffuse, float shininess );

      Material( const Material& material ) = default;

      // All vars public for easier access. Make private
      Color baseColor;
      // Cached value of diffuse * baseColor for faster Phong reflexion model calculation
      Color diffuseColor;
      float specular;
      float diffuse;
      float shininess;
};
#endif //SEQUENCIAL_MATERIAL_H
