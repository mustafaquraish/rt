#pragma once

#include <core/integrator.h>

struct AmbientOcclusion : Integrator {
  AmbientOcclusion(RenderParams params) : Integrator(params) {
    gammaCorrect = false;
  };
  Colour Li(Ray& r, Scene *scene, RNG& rng);
};

