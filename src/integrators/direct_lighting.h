#ifndef __DIRECT_LIGHTING_H__
#define __DIRECT_LIGHTING_H__

#include "core/integrator.h"

struct DirectLighting : Integrator {
  DirectLighting(RenderParams params) : Integrator(params) {};
  Colour SampleLight(HitRec& rec, Scene *scene, RNG& rng);
  Colour Li(Ray& r, Scene *scene, RNG& rng);
};

#endif // __DIRECT_LIGHTING_H__
