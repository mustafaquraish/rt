#include "core/rt.h"

int main(int argc, char **argv) {
  RenderParams params(argc, argv);
  if (!params.exists("width"))   params.setInt("width", 512);
  if (!params.exists("height"))  params.setInt("height", 512);
  if (!params.exists("samples")) params.setInt("samples", 10);
  if (!params.exists("output"))  params.setStr("output", "output.ppm");

  // if (!params.exists("scene")) params.setStr("scene", "A4");
  // if (!params.exists("scene")) params.setStr("scene", "SOR");
  // if (!params.exists("scene")) params.setStr("scene", "Ajax");
  if (!params.exists("scene")) params.setStr("scene", "Cornell");
  // if (!params.exists("scene")) params.setStr("scene", "Implicit");
  // if (!params.exists("scene")) params.setStr("scene", "Lsystems");
  // if (!params.exists("scene")) params.setStr("scene", "Occlusion");

  Scene *scene = RTSceneFactory::Create(params);

  if (scene->integrator) 
    scene->integrator->render(scene, 10);
  return 0;
}