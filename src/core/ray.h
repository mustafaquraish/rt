#ifndef __RAY_H__
#define __RAY_H__

#include "core/math.h"

struct Ray {
  Ray(Vec p, Vec d): p(p), d(d), tMax(INFINITY) {}
  Ray(Vec p, Vec d, double tMax): p(p), d(d), tMax(tMax) {}

  inline Vec at(double lambda) const { return p + lambda * d; }

  Vec p;
  Vec d;
  double tMax = INFINITY;
};

#endif // __RAY_H__
