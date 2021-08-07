#include <core/scene.h>
#include <core/renderer.h>
#include <util/sysutil.h>

int main(int argc, char **argv) {
  RenderParams params(argc, argv);
  
  if (!params.get<bool>("animate")) {

    Scene *scene = RTSceneFactory::Create(params);
    if (scene->renderer) scene->renderer->render(scene);
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
      params.update_frame(frame);

      Scene *scene = RTSceneFactory::Create(params);
      if (scene->renderer) scene->renderer->render(scene);
      delete scene;
      
      run_command("convert temp.ppm frames/%03d.png", frame);
    }
    run_command("convert frames/*.png %s", origOut);
  }

  return 0;
}