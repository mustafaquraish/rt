#include "core/rt.h"

Colour trace(Ray& ray, Scene* scene) {
  return Vec();
}

int main() {
  Scene *scene = A2Scene(1024, 1024);
  Ray kk = Ray(Vec(), Vec());
  if (scene->integrator) scene->integrator->render(scene, 10);
  else {
    cout << "No integrator set in scene, using DebugShader" << endl;
    DebugShader ns;
    ns.render(scene, 10);
  }
  return 0;
}