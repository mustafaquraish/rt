#pragma once

#include <core/renderer.h>

struct HitRec;

struct DirectLighting : Renderer {
  DirectLighting(RenderParams params) : Renderer(params) {};
  Colour Li(Ray& r, Scene *scene, RNG& rng);
};

