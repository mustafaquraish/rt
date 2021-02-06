#ifndef __INTEGRATOR_H__
#define __INTEGRATOR_H__

#include "core/rng.h"
#include "core/ray.h"

struct Scene;

struct Integrator {
  virtual void render(Scene *scene, int depth) = 0;
  virtual Colour Li(Ray& r, Scene *scene, RNG& rng) { return Vec(0); };
};

#endif // __INTEGRATOR_H__
