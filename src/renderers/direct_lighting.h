#pragma once

#include <core/renderer.h>

struct DirectLighting : Renderer {
  DirectLighting(RenderParams params) : Renderer(params) {};
  Colour SampleLight(HitRec& rec, Scene *scene, RNG& rng);
  Colour Li(Ray& r, Scene *scene, RNG& rng);
};

