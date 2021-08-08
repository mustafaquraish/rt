#pragma once

#include <core/math.h>

struct Ray {
  Ray(Vec3 p, Vec3 d): p(p), d(d), tMax(INFINITY) {}
  Ray(Vec3 p, Vec3 d, float tMax): p(p), d(d), tMax(tMax) {}

  inline Vec3 at(float lambda) const { return p + lambda * d; }

  Vec3 p;
  Vec3 d;
  float tMax = INFINITY;
};

