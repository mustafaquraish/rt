#pragma once

#include <core/renderer.h>

enum DebugType{DEBUG_NORMALS, DEBUG_DEPTH, DEBUG_COLOUR};

struct DebugShader : Renderer {
  DebugShader(RenderParams& params, DebugType type=DEBUG_NORMALS) 
    : Renderer(params), type(type) {
    enableRNG = false;
    gammaCorrect = false;
    maxSamples = 1;
  };

  Colour Li(Ray &r, Scene *scene, RNG& rng);

  DebugType type;
};

