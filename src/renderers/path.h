#pragma once

#include <core/renderer.h>

struct HitRec;
struct Path : Renderer {
  Path(RenderParams params) : Renderer(params) {};
  Colour SampleLight(HitRec& rec, Scene *scene, RNG& rng);
  Colour Li(Ray& r, Scene *scene, RNG& rng);
};
 
