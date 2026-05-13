#include "GPURayTracer.cuh"
#include "lodepng.h"
#include <memory>
#include <chrono>
#include <iostream>

float4 makeFloat4( float x, float y, float z )
{
   return float4{ x, y, z, 0.f };
}

int main()
{
   static constexpr int RGBABytes = 4;

   TracerOptions options;
   options.fieldOfView = 90;
   options.cameraDistance = 50;
   options.imageWidth = 1280;
   options.imageHeight = 720;
   options.backgroundColor = Color( 0.01f, 0.01f, 0.01f );
   options.ambientLightColor = Color( 0.1f, 0.1f, 0.1f );

   GPURayTracer rayTracer;

   std::vector<SceneObject> objects;
   std::vector<Light> lights;

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

   auto start = std::chrono::high_resolution_clock::now();

   auto image = GPURayTracer::generateRawImage( options, objects, lights );

   auto end = std::chrono::high_resolution_clock::now();
   auto duration = std::chrono::duration_cast<std::chrono::milliseconds>( end - start );
   std::cout << "Image generation took " << duration.count() << " ms" << std::endl;
   std::cout << "Image is: " << options.imageWidth << "x" << options.imageHeight << " == " << image.size() / RGBABytes <<
         " pixels and " << image.size() << " bytes." <<
         std::endl;

   /*
   // Convert to lodepng format
   std::vector<unsigned char> image( pixels.size() * RGBABytes );

   for( size_t i = 0; i < pixels.size(); ++i )
   {
      image[ i * RGBABytes + 0 ] = pixels[ i ].R;
      image[ i * RGBABytes + 1 ] = pixels[ i ].G;
      image[ i * RGBABytes + 2 ] = pixels[ i ].B;
      image[ i * RGBABytes + 3 ] = pixels[ i ].alpha;
   }
*/
   // Generating png
   lodepng::encode( "output.png", image, options.imageWidth, options.imageHeight );
}
