#pragma once

#include <core/renderer.h>

struct HitRec;

Colour SampleLight(HitRec& rec, Scene *scene, RNG& rng);

struct Path : Renderer {
  Path(RenderParams params) : Renderer(params) {};
  Colour Li(Ray& r, Scene *scene, RNG& rng);
};
 
