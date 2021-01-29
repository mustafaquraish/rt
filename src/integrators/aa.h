#ifndef __AA_SHADER_H__
#define __AA_SHADER_H__

#include "core/integrator.h"

struct AmbientOcclusion : Integrator {
  void render(Scene *scene, int depth);
};

#endif // __AA_SHADER_H__
