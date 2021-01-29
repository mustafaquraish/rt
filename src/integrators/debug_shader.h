#ifndef __NORMAL_SHADER_H__
#define __NORMAL_SHADER_H__

#include "core/integrator.h"

enum DebugType{DEBUG_NORMALS, DEBUG_DEPTH, DEBUG_COLOUR};

struct DebugShader : Integrator {
  DebugShader(DebugType type=DEBUG_NORMALS) : type(type) {}
  void render(Scene *scene, int depth);
  DebugType type;
};

#endif // __NORMAL_SHADER_H__
