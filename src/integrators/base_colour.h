#pragma once

#include <core/integrator.h>
#include <util/params.h>
#include <core/primitive.h>

struct BaseColour : Integrator {
  BaseColour(RenderParams params) : Integrator(params) {
    gammaCorrect = false;
    enableRNG = false;
  };
  Colour Li(Ray& r, Scene *scene, RNG& rng);
};

