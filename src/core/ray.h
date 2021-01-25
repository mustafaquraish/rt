#ifndef __RAY_H__
#define __RAY_H__

#include "core/math.h"

struct Ray {
  Ray() {}
  Ray(Vec p, Vec d): p(p), d(d) {}

  inline Vec at(double lambda) const { return p + lambda * d; }

  Vec p;
  Vec d;
  Vec I = Vec(0,0,0);
  Vec C = Vec(1,1,1);
  int eidx = -1;
};

#endif // __RAY_H__
