#include "RayTracer.h"
#include "Levels.h"
#include "lodepng.h"
#include <memory>
#include <chrono>
#include <iostream>

void chooseLevel( int levelID, TracerOptions& options, std::vector<std::shared_ptr<SceneObject>>& objects, std::vector<Light>& lights )
{
   // TODO ugly, use map
   switch( levelID )
   {
      case 1:
         BasicLevel().loadLevel( options, objects, lights );
         break;
      case 2:
         LightColors().loadLevel( options, objects, lights );
         break;
      case 3:
         HighResLights().loadLevel( options, objects, lights );
         break;
      case 4:
         LightCombination().loadLevel( options, objects, lights );
         break;
      case 5:
         Space().loadLevel( options, objects, lights );
         break;
      default:
         throw std::runtime_error( "Invalid level ID" );
   }
}

int main( int argc, char** argv ){
   static constexpr int RGBABytes = 4;

   TracerOptions options;
   if( argc != 2 )
   {
      std::cout << "Usage: " << argv[ 0 ] << " <level ID>" << std::endl;
      std::cout << "Available levels: 1, 2, 3, 4, 5, 6" << std::endl;
      return -1;
   }

   // TODO better invalid string handling
   int levelID = std::stoi( argv[ 1 ] );

   RayTracer rayTracer;

   std::vector<std::shared_ptr<SceneObject>> objects;
   std::vector<Light> lights;
   chooseLevel( levelID, options, objects, lights );

   auto start = std::chrono::high_resolution_clock::now();

   auto image = RayTracer::generateRawImage( options, objects, lights );

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
