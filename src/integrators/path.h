#pragma once

#include <core/integrator.h>
#include <util/params.h>
#include <core/primitive.h>

struct Path : Integrator {
  Path(RenderParams params) : Integrator(params) {};
  Colour SampleLight(HitRec& rec, Scene *scene, RNG& rng);
  Colour Li(Ray& r, Scene *scene, RNG& rng);
};
 
