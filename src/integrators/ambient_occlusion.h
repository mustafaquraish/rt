#ifndef __AA_SHADER_H__
#define __AA_SHADER_H__

#include "core/integrator.h"

struct AmbientOcclusion : Integrator {
  AmbientOcclusion(RenderParams params) : Integrator(params) {
    gammaCorrect = false;
  };
  Colour Li(Ray& r, Scene *scene, RNG& rng);
};

#endif // __AA_SHADER_H__
