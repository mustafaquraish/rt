#pragma once

#include <core/renderer.h>
#include <util/params.h>
#include <core/primitive.h>

struct BaseColour : Renderer {
  BaseColour(RenderParams params) : Renderer(params) {
    gammaCorrect = false;
    enableRNG = false;
  };
  Colour Li(Ray& r, Scene *scene, RNG& rng);
};

