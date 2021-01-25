#ifndef __INTEGRATOR_H__
#define __INTEGRATOR_H__

struct Scene;

struct Integrator {
  virtual void render(Scene *scene, int depth) = 0;
};

#endif // __INTEGRATOR_H__
