#ifndef __DIRECT_LIGHTING_H__
#define __DIRECT_LIGHTING_H__

#include "core/integrator.h"
#include "util/params.h"
#include "core/primitive.h"

struct DirectLighting : Integrator {
  DirectLighting(RenderParams params) : Integrator(params) {};
  Colour SampleLight(HitRec& rec, Scene *scene, RNG& rng);
  void render(Scene *scene, int depth);
  Colour Li(Ray& r, Scene *scene, RNG& rng);
};

#endif // __DIRECT_LIGHTING_H__
