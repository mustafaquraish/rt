#pragma once

#include <core/renderer.h>

struct HitRec;
struct Object;

Colour SampleLight(HitRec& rec, Scene *scene, RNG& rng, Object ** sampledLight);

struct Path : Renderer {
  Path(RenderParams params) : Renderer(params) {};
  Colour Li(Ray& r, Scene *scene, RNG& rng);
};
 
