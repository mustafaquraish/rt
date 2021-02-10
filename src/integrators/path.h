#ifndef __PATH_H__
#define __PATH_H__

#include "core/integrator.h"
#include "util/params.h"

struct Path : Integrator {
  Path(RenderParams params) : Integrator(params) {};
  void render(Scene *scene, int depth);
  Colour Li(Ray& r, Scene *scene, RNG& rng);
};

#endif // __PATH_H__
