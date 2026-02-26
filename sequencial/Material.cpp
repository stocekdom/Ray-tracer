//
// Created by dominik on 20.02.26.
//
#include "Material.h"

Material::Material() : specular( 0 ), diffuse( 0 ), shininess( 0 )
{
}

Material::Material( const Color& baseColor, float specular, float diffuse, float shininess )
   : baseColor( baseColor ),
     specular( specular ),
     diffuse( diffuse ),
     shininess( shininess )
{
}
