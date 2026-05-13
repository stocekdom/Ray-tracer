//
// Created by dominik on 13.05.26.
//

#include "Levels.h"

float4 makeFloat4( float x, float y, float z )
{
   return float4{ x, y, z, 0.f };
}

void BasicLevel::loadLevel( TracerOptions& options, std::vector<SceneObject>& objects, std::vector<Light>& lights )
{
   options.fieldOfView = 90;
   options.cameraDistance = 50;
   options.imageWidth = 1280;
   options.imageHeight = 720;
   options.backgroundColor = Color( 0.01f, 0.01f, 0.01f );
   options.ambientLightColor = Color( 0.1f, 0.1f, 0.1f );

   Material floor( Color( 0.95f, 0.05f, 0.05f ), 0.1f, 0.9f, 16.f );
   Material orangeMaterial( Color( 1.f, 0.5f, 0.05f ), 0.45f, 0.3f, 64.f );
   Material greenMaterial( Color( 0.05f, 1.f, 0.01f ), 0.45f, 0.3f, 64.f );
   objects.emplace_back( SceneObject::makeSphere( makeFloat4( -10.f, -10.f, 100.f ), orangeMaterial, 22.f ) );
   objects.emplace_back(
      SceneObject::makeBlock( makeFloat4( 50.f, -30.f, 110.f ), greenMaterial, makeFloat4( 12.f, 10.f, 15.f ) ) );

   objects.emplace_back(
      SceneObject::makePlane( makeFloat4( 0.f, -50.f, 100.f ), floor, makeFloat4( 0.f, 1.f, 0.f ), 100.f, 100.f ) );

   objects.emplace_back(
      SceneObject::makePlane( makeFloat4( -120.f, 10.f, 100.f ), floor, makeFloat4( 1.f, 0.f, 0.f ), 100.f, 100.f ) );

   lights.emplace_back( makeFloat4( 30.f, 20.f, 10.f ), Color( 0.98f, 0.95f, 0.90f ), 4.f );
}

void LightColors::loadLevel( TracerOptions& options, std::vector<SceneObject>& objects, std::vector<Light>& lights )
{
   options.fieldOfView = 90;
   options.cameraDistance = 60;
   options.imageWidth = 1920;
   options.imageHeight = 1080;
   options.backgroundColor = Color( 0.01f, 0.01f, 0.01f );
   options.ambientLightColor = Color( 0.12f, 0.15f, 0.12f );

   Material floor( Color( 0.85f, 0.05f, 0.05f ), 0.1f, 0.8f, 16.f );
   Material walls( Color( 0.97f, 1.f, 0.97f ), 0.25f, 0.7f, 32.f );
   Material object( Color( 0.75f, 0.75f, 0.75f ), 0.5f, 0.25f, 64.f );

   objects.emplace_back(
      SceneObject::makePlane( makeFloat4( 0.f, -70.f, 110.f ), floor, makeFloat4( 0.f, 1.f, 0.f ), 100.f, 100.f ) );

   objects.emplace_back(
      SceneObject::makePlane( makeFloat4( -200.f, 10.f, 100.f ), walls, makeFloat4( 1.f, 0.f, 0.f ), 100.f, 100.f ) );
   objects.emplace_back(
      SceneObject::makePlane( makeFloat4( 200.f, 10.f, 100.f ), walls, makeFloat4( -1.f, 0.f, 0.f ), 100.f, 100.f ) );
   objects.emplace_back(
      SceneObject::makePlane( makeFloat4( 0.f, 10.f, 200.f ), walls, makeFloat4( 0.f, 0.f, -1.f ), 100.f, 100.f ) );

   objects.emplace_back(
      SceneObject::makeBlock( makeFloat4( -80.f, -30.f, 110.f ), object, makeFloat4( 15.f, 10.f, 15.f ) ) );
   objects.emplace_back(
      SceneObject::makeSphere( makeFloat4( 10.f, -30.f, 110.f ), object, 25.f ) );

   lights.emplace_back( makeFloat4( 180.f, 35.f, 10.f ), Color( 0.1f, 1.f, 0.1f ), 4.6f );
   lights.emplace_back( makeFloat4( -180.f, 35.f, 10.f ), Color( 1.f, 0.12f, 0.1f ), 4.6f );
}

void HighResLights::loadLevel( TracerOptions& options, std::vector<SceneObject>& objects, std::vector<Light>& lights )
{
   options.fieldOfView = 90;
   options.cameraDistance = 60;
   options.imageWidth = 3840;
   options.imageHeight = 2160;
   options.backgroundColor = Color( 0.01f, 0.01f, 0.01f );
   options.ambientLightColor = Color( 0.15f, 0.12f, 0.15f );

   Material white( Color( 0.9f, 0.9f, 0.9f ), 0.65f, 0.25f, 64.f );
   Material walls( Color( 0.65f, 0.2f, 0.4f ), 0.25f, 0.7f, 32.f );

   lights.emplace_back( makeFloat4( -180.f, 32.f, 20.f ), Color( 0.14f, 0.1f, 1.f ), 4.7f );
   lights.emplace_back( makeFloat4( 180.f, 35.f, 20.f ), Color( 0.1f, 1.f, 0.16f ), 4.7f );
   lights.emplace_back( makeFloat4( 10.f, 40.f, -10.f ), Color( 1.f, 0.12f, 0.16f ), 4.7f );

   objects.emplace_back(
      SceneObject::makeSphere( makeFloat4( -70.f, 10.f, 100.f ), white, 18.f ) );
   objects.emplace_back(
      SceneObject::makeSphere( makeFloat4( 80.f, 15.f, 110.f ), white, 22.f ) );

   objects.emplace_back(
      SceneObject::makePlane( makeFloat4( 0.f, -60.f, 0.f ), walls, makeFloat4( 0.f, 1.f, 0.f ), 100.f, 100.f ) );
   objects.emplace_back(
      SceneObject::makePlane( makeFloat4( 0.f, 10.f, 400.f ), walls, makeFloat4( 0.f, 0.f, -1.f ), 100.f, 100.f ) );
}

void LightCombination::loadLevel( TracerOptions& options, std::vector<SceneObject>& objects, std::vector<Light>& lights )
{
   options.fieldOfView = 90;
   options.cameraDistance = 60;
   options.imageWidth = 1920;
   options.imageHeight = 1080;
   options.backgroundColor = Color( 0.01f, 0.01f, 0.01f );
   options.ambientLightColor = Color( 0.15f, 0.1f, 0.15f );

   Material blue( Color( 0.1f, 0.05f, 0.75f ), 0.55f, 0.3f, 64.f );
   Material white( Color( 0.75f, 0.7f, 0.75f ), 0.45f, 0.25f, 32.f );
   Material walls( Color( 0.8f, 0.2f, 0.6f ), 0.3f, 0.65f, 16.f );

   lights.emplace_back( makeFloat4( 10.f, 35.f, -10.f ), Color( 0.1f, 1.f, 0.05f ), 4.6f );
   lights.emplace_back( makeFloat4( 11.f, 40.f, -10.f ), Color( 1.f, 0.1f, 0.05f ), 4.6f );

   objects.emplace_back(
      SceneObject::makeBlock( makeFloat4( -50.f, -20.f, 110.f ), blue, makeFloat4( 15.f, 10.f, 20.f ) ) );
   objects.emplace_back(
      SceneObject::makeSphere( makeFloat4( 50.f, 10.f, 130.f ), white, 20.f ) );

   objects.emplace_back(
      SceneObject::makePlane( makeFloat4( 0.f, -60.f, 0.f ), walls, makeFloat4( 0.f, 1.f, 0.f ), 100.f, 100.f ) );
   objects.emplace_back(
      SceneObject::makePlane( makeFloat4( 0.f, 10.f, 400.f ), walls, makeFloat4( 0.f, 0.f, -1.f ), 100.f, 100.f ) );
}

void Space::loadLevel( TracerOptions& options, std::vector<SceneObject>& objects, std::vector<Light>& lights )
{
   options.fieldOfView = 90;
   options.cameraDistance = 45;
   options.imageWidth = 3840;
   options.imageHeight = 2160;
   options.backgroundColor = Color( 0.01f, 0.01f, 0.01f );
   options.ambientLightColor = Color( 0.1f, 0.1f, 0.1f );

   lights.emplace_back( makeFloat4( 0.f, 25.f, 120.f ), Color( 0.95f, 0.75f, 0.03f ), 4.6f );
   Material blue( Color( 0.1f, 0.2f, 0.75f ), 0.5f, 0.3f, 64.f );
   Material red( Color( 0.85f, 0.05f, 0.15f ), 0.4, 0.25f, 32.f );
   Material orange( Color( 0.5f, 0.25f, 0.05f ), 0.25f, 0.55f, 16.f );

   objects.emplace_back(
    SceneObject::makeSphere( makeFloat4( 100.f, 10.f, 85.f ), blue, 10.f ) );
   objects.emplace_back(
    SceneObject::makeSphere( makeFloat4( -35.f, 15.f, 160.f ), blue, 18.f ) );
   objects.emplace_back(
    SceneObject::makeSphere( makeFloat4( 45.f, 6.f, 60.f ), red, 12.f ) );
   objects.emplace_back(
    SceneObject::makeSphere( makeFloat4( -120.f, 0.f, 85.f ), orange, 24.f ) );
   objects.emplace_back(
   SceneObject::makeSphere( makeFloat4( 0.f, 20.f, 124.f ), orange, 2.f ) );
}
