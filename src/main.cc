#include "core/scene.h"
#include "core/integrator.h"


int main(int argc, char **argv) {
  RenderParams params(argc, argv);
  
  if (!params.get<bool>("animate")) {

    Scene *scene = RTSceneFactory::Create(params);
    if (scene->integrator) scene->integrator->render(scene);
    delete scene;

  } else {

    int frameBegin = params.get<int>("frameBegin");
    int frameEnd = params.get<int>("frameEnd");

    char origOut[1024];
    const char *of = params.get<char *>("output");
    strcpy(origOut, of);
    printf("Orig output is : %s\n", origOut);


    char buffer[1024];
    params.set<const char *>("output", "temp.ppm");
    for (int frame = frameBegin; frame < frameEnd; frame++) {
      printf("\n================ Frame %d ===================\n", frame);
      params.update(frame);

      Scene *scene = RTSceneFactory::Create(params);
      if (scene->integrator) scene->integrator->render(scene);
      delete scene;
      
      sprintf(buffer, "convert temp.ppm frames/%02d.png", frame);
      if (system(buffer)) printf("Error with `system(%s)`", buffer);
    }
    sprintf(buffer, "convert frames/*.png %s", origOut);
    if (system(buffer)) printf("Error with `system(%s)`", buffer);
  }

  return 0;
}