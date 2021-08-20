#pragma once

#include <util/params.h>
#include <core/colour.h>

struct Ray;
struct RNG;
struct Scene;

struct Renderer {
  Renderer(RenderParams& params) : params(params) {
    if (params.exists("exposure"))
      exposure = params.get<float>("exposure");
  };
  virtual ~Renderer() {};

  virtual void render(Scene *scene);
  virtual Colour Li(Ray& r, Scene *scene, RNG& rng) { return Vec3(0); };

  RenderParams params;
  bool enableRNG = true;
  bool gammaCorrect = true;
  float exposure = 1.0;
  int maxSamples = INT32_MAX;
};

