#include "core/rt.h"

Scene *CornellScene(int sx, int sy);


int main(int argc, char **argv) {
  int resolution = 1024;
  if (argc > 1) resolution = atoi(argv[1]);

  // Scene *scene = A2Scene(resolution, resolution);
  // Scene *scene = SORScene(resolution, resolution);
  // Scene *scene = AjaxScene(resolution, resolution);
  Scene *scene = CornellScene(resolution, resolution);
  // Scene *scene = ImplicitScene(resolution, resolution);
  // Scene *scene = RoomScene(resolution, resolution);
  // Scene *scene = OcclusionScene(resolution, resolution);
  // Scene *scene = LsystemsScene(resolution, resolution);

  Ray kk = Ray(Vec(), Vec());
  if (scene->integrator) scene->integrator->render(scene, 10);
  else {
    cout << "No integrator set in scene, using DebugShader" << endl;
    DebugShader ns;
    ns.render(scene, 10);
  }
  return 0;
}