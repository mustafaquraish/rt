#include "core/scene.h"
#include "core/integrator.h"

#define ANIMATION

int main(int argc, char **argv) {
  RenderParams params(argc, argv);
  if (!params.exists("width"))   params.setInt("width", 512);
  if (!params.exists("height"))  params.setInt("height", 512);
  if (!params.exists("samples")) params.setInt("samples", 10);
  if (!params.exists("output"))  params.setStr("output", "output.ppm");

  if (!params.exists("scene")) params.setStr("scene", "Cornell");

  
  
  
#ifndef ANIMATION
  Scene *scene = RTSceneFactory::Create(params);
  if (scene->integrator) scene->integrator->render(scene, 10);
  delete scene;

#else

  char outputStr[1024];
  // Animation
  for (int i = 0; i < 60; i++) {
    params.setInt("frame", i);
    sprintf(outputStr, "temp.ppm");
    params.setStr("output", outputStr);
    Scene *scene = RTSceneFactory::Create(params);
    if (scene->integrator) scene->integrator->render(scene, 10);
    sprintf(outputStr, "convert temp.ppm frames/%02d.png", i);
    system(outputStr);
    delete scene;
  }
  system("convert frames/*.png output.gif");
#endif

  return 0;
}