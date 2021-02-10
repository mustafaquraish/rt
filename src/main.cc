#include "core/rt.h"

int main(int argc, char **argv) {
  RenderParams params(argc, argv);
  if (!params.exists("resolution")) params.setInt("resolution", 512);
  if (!params.exists("samples"))    params.setInt("samples", 10);
  if (!params.exists("output"))     params.setStr("output", "test.ppm");

  // Scene *scene = SceneFactory::Create("A2", params);
  // Scene *scene = SceneFactory::Create("SOR", params);
  // Scene *scene = SceneFactory::Create("Ajax", params);
  // Scene *scene = SceneFactory::Create("Cornell", params);
  // Scene *scene = SceneFactory::Create("Implicit", params);
  // Scene *scene = SceneFactory::Create("Lsystems", params);
  Scene *scene = SceneFactory::Create("Occlusion", params);

  Ray kk = Ray(Vec(), Vec());
  if (scene->integrator) 
    scene->integrator->render(scene, 10);

  return 0;
}