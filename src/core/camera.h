#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "core/rng.h"
#include "core/ray.h"
#include "util/params.h"

struct Camera {
  Camera() {};
  Camera(Vec e, Vec g, Vec u, double vfov, RenderParams& params);

  virtual Ray getRay(int i, int j, RNG& rng);
  virtual Ray getRay(int i, int j);
  
  Vec e;
  double du, dv, wl, wt;
  Matrix C2W, W2C;

  bool DOF = false;
  double aperture, focus_dist;
};

#endif // __CAMERA_H__
