#include "core/scene.h"
#include "core/integrator.h"

int main(int argc, char **argv) {
  RenderParams params(argc, argv);
  if (!params.exists("width"))   params.setInt("width", 512);
  if (!params.exists("height"))  params.setInt("height", 512);
  if (!params.exists("samples")) params.setInt("samples", 10);
  if (!params.exists("output"))  params.setStr("output", "output.ppm");

  if (!params.exists("scene")) params.setStr("scene", "Cornell");

  Scene *scene = RTSceneFactory::Create(params);

  if (scene->integrator) scene->integrator->render(scene, 10);

  delete scene;
  return 0;
}