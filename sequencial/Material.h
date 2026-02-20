//
// Created by dominik on 20.02.26.
//

#ifndef SEQUENCIAL_MATERIAL_H
#define SEQUENCIAL_MATERIAL_H

#include "Color.h"

class Material
{
   Material();

   Material( const Material& ) = default;

   Material( Color baseColor, Color ambientColor, float specular, float diffuse, float shininess );

   Color baseColor;
   Color ambientColor;
   float specular;
   float diffuse;
   float shininess;
};
#endif //SEQUENCIAL_MATERIAL_H
