//
// Created by dominik on 20.02.26.
//

#ifndef SEQUENCIAL_MATERIAL_H
#define SEQUENCIAL_MATERIAL_H

#include "Color.h"

class Material
{
   public:
      Material();

      Material( const Color& baseColor, const Color& ambientColor, float specular, float diffuse, float shininess );

      Material( const Material& material ) = default;

      Color baseColor;
      Color ambientColor;
      float specular;
      float diffuse;
      float shininess;
};
#endif //SEQUENCIAL_MATERIAL_H
