#pragma once

#include <core/renderer.h>

struct BaseColour : Renderer {
  BaseColour(RenderParams params) : Renderer(params) {
    gammaCorrect = false;
    enableRNG = false;
  };
  Colour Li(Ray& r, Scene *scene, RNG& rng);
};

