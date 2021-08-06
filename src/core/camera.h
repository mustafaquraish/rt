#pragma once

#include <core/rng.h>
#include <core/ray.h>
#include <util/params.h>

struct Camera {
  Camera() {};
  Camera(Vec3 e, Vec3 g, Vec3 u, double vfov, RenderParams& params);

  virtual Ray getRay(int i, int j, RNG& rng);
  virtual Ray getRay(int i, int j);
  
  Vec3 e;
  double du, dv, wl, wt;
  Matrix4 C2W, W2C;

  bool DOF = false;
  double aperture, focus_dist;
};

