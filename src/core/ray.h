#pragma once

#include <core/math.h>

struct Ray {
  Ray(Vec3 p, Vec3 d): p(p), d(d), tMax(INFINITY) {}
  Ray(Vec3 p, Vec3 d, double tMax): p(p), d(d), tMax(tMax) {}

  inline Vec3 at(double lambda) const { return p + lambda * d; }

  Vec3 p;
  Vec3 d;
  double tMax = INFINITY;
};

