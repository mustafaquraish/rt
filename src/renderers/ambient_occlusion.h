#pragma once

#include <core/renderer.h>

struct AmbientOcclusion : Renderer {
  AmbientOcclusion(RenderParams params) : Renderer(params) {
    gammaCorrect = false;
  };
  Colour Li(Ray& r, Scene *scene, RNG& rng);
};

