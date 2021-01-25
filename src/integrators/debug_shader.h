#ifndef __NORMAL_SHADER_H__
#define __NORMAL_SHADER_H__

#include "core/integrator.h"

struct DebugShader : Integrator {
  void render(Scene *scene, int depth);
};

#endif // __NORMAL_SHADER_H__
