#pragma once

#include <core/integrator.h>

enum DebugType{DEBUG_NORMALS, DEBUG_DEPTH, DEBUG_COLOUR};

struct DebugShader : Integrator {
  DebugShader(RenderParams& params, DebugType type=DEBUG_NORMALS) 
    : Integrator(params), type(type) {
    enableRNG = false;
    gammaCorrect = false;
    maxSamples = 1;
  };

  Colour Li(Ray &r, Scene *scene, RNG& rng);

  DebugType type;
};

