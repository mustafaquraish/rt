#pragma once

#include <core/integrator.h>

struct DirectLighting : Integrator {
  DirectLighting(RenderParams params) : Integrator(params) {};
  Colour SampleLight(HitRec& rec, Scene *scene, RNG& rng);
  Colour Li(Ray& r, Scene *scene, RNG& rng);
};

