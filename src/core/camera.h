#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "core/math.h"
#include "core/ray.h"

struct Camera {
  Camera() {}
  Camera(Vec e, Vec g, Vec u, double f, double wl, double wt,
         double wsize, int sx, int sy);

  virtual Ray getRay(int i, int j);
  
  Vec e, u, v, w;
  double du, dv, wl, wt, f, wsize;
  Matrix C2W, W2C;
};

#endif // __CAMERA_H__
