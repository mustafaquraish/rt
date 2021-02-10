#ifndef __DEBUG_SHADER_H__
#define __DEBUG_SHADER_H__

#include "core/integrator.h"

enum DebugType{DEBUG_NORMALS, DEBUG_DEPTH, DEBUG_COLOUR};

struct DebugShader : Integrator {
  DebugShader(RenderParams& params, DebugType type=DEBUG_NORMALS) 
    : Integrator(params), type(type) {};
  void render(Scene *scene, int depth);

  DebugType type;
};

#endif // __DEBUG_SHADER_H__
