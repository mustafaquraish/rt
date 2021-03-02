#ifndef __INTEGRATOR_H__
#define __INTEGRATOR_H__

#include "core/rng.h"
#include "core/ray.h"
#include "core/object.h"
#include "core/scene.h"
#include "util/params.h"
#include "time.h"

struct Scene;

struct Integrator {
  Integrator(RenderParams& params) : params(params) {};
  virtual ~Integrator() {};

  virtual void render(Scene *scene);
  virtual Colour Li(Ray& r, Scene *scene, RNG& rng) { return Vec(0); };

  RenderParams params;
  bool enableRNG = true;
  bool gammaCorrect = true;
  int maxSamples = INT32_MAX;
};

#endif // __INTEGRATOR_H__
