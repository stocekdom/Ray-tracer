//
// Created by dominik on 13.05.26.
//

#ifndef GPURAYTRACER_LEVELS_H
#define GPURAYTRACER_LEVELS_H
#include "Objects.cuh"
#include "TracerOptions.h"
#include <vector>

// TODO add level loading from a JSON
class Level
{
   public:
      virtual ~Level() = default;

      virtual void loadLevel( TracerOptions& options, std::vector<SceneObject>& objects, std::vector<Light>& lights ) = 0;
};

class BasicLevel : public Level
{
   public:
      void loadLevel( TracerOptions& options, std::vector<SceneObject>& objects, std::vector<Light>& lights ) override;
};

class LightColors : public Level
{
   public:
      void loadLevel( TracerOptions& options, std::vector<SceneObject>& objects, std::vector<Light>& lights ) override;
};

class HighResLights : public Level
{
   public:
      void loadLevel( TracerOptions& options, std::vector<SceneObject>& objects, std::vector<Light>& lights ) override;
};

class LightCombination : public Level
{
   public:
      void loadLevel( TracerOptions& options, std::vector<SceneObject>& objects, std::vector<Light>& lights ) override;
};

class Space : public Level
{
   public:
      void loadLevel( TracerOptions& options, std::vector<SceneObject>& objects, std::vector<Light>& lights ) override;
};

#endif //GPURAYTRACER_LEVELS_H
