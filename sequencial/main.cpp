#include "RayTracer.h"
#include "lodepng.h"
#include <memory>
#include <chrono>
#include <iostream>

int main()
{
   static constexpr int RGBABytes = 4;

   TracerOptions options;
   options.fieldOfView = 90;
   options.cameraDistance = 50;
   options.imageWidth = 1280;
   options.imageHeight = 720;
   options.backgroundColor = Color( 10.0f, 10.0f, 10.0f );

   RayTracer rayTracer;

   std::vector<std::shared_ptr<SceneObject>> objects;

   Material floor( Color( 220.f, 200.f, 220.f ), Color( 0.f, 0.f, 0.f ), 1.f, 0.5f, 4.f );
   Material orangeMaterial( Color( 220.f, 150.f, 20.f ), Color( 10.f, 5.f, 5.f ), 1.f, 0.5f, 4.f );
   Material greenMaterial( Color( 20.f, 230.f, 10.f ), Color( 5.f, 10.f, 5.f ), 1.f, 0.5f, 4.f );
   objects.emplace_back( std::make_shared<Sphere>( Vector3f{ -20.f, 10.f, 100.f }, greenMaterial, 5.f ) );
   objects.emplace_back( std::make_shared<Sphere>( Vector3f{ 0.f, 0.f, 220.f }, orangeMaterial, 45.f ) );
   objects.emplace_back( std::make_shared<Plane>( Vector3f( 0.f, -30.f, 0.f ), floor, Vector3f( 0.f, 1.f, 0.f ), 100.f, 100.f ) );

   auto start = std::chrono::high_resolution_clock::now();

   auto image = RayTracer::generateRawImage( options, objects, {} );

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
