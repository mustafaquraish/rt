#pragma once

#include <core/ray.h>

struct RenderParams;
struct RNG;

struct Camera {
  Camera() {};
  Camera(Vec3 e, Vec3 g, Vec3 u, float vfov, RenderParams& params);

  virtual Ray getRay(int i, int j, RNG& rng);
  virtual Ray getRay(int i, int j);
  
  Vec3 e;
  float du, dv, wl, wt;
  Matrix4 C2W, W2C;

  bool DOF = false;
  float aperture, focus_dist;
};

