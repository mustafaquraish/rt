#ifndef __PATH_H__
#define __PATH_H__

#include "core/integrator.h"

struct Path : Integrator {
  void render(Scene *scene, int depth);
};

#endif // __PATH_H__
