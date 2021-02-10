#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "core/rng.h"
#include "core/ray.h"
#include "util/params.h"

struct Camera {
  Camera() {}
  Camera(Vec e, Vec g, Vec u, double f, RenderParams& params);

  virtual Ray getRay(int i, int j, RNG& rng);
  virtual Ray getRay(int i, int j);
  
  Vec e, u, v, w;
  double du, dv, wl, wt, f;
  Matrix C2W, W2C;
};

#endif // __CAMERA_H__
