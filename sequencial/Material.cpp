//
// Created by dominik on 20.02.26.
//
#include "Material.h"

Material::Material() : specular( 0 ), diffuse( 0 ), shininess( 0 )
{
}

Material::Material( Color baseColor, Color ambientColor, float specular, float diffuse, float shininess )
   : baseColor( baseColor ), ambientColor( ambientColor ), specular( specular ), diffuse( diffuse ), shininess( shininess )
{
}
