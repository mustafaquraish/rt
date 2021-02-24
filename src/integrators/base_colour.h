#ifndef __BASE_COLOUR_H__
#define __BASE_COLOUR_H__

#include "core/integrator.h"
#include "util/params.h"
#include "core/primitive.h"

struct BaseColour : Integrator {
  BaseColour(RenderParams params) : Integrator(params) {};
  void render(Scene *scene, int depth);
  Colour Li(Ray& r, Scene *scene, RNG& rng);
};

#endif // __BASE_COLOUR_H__