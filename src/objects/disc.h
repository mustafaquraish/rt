#ifndef __DISC_H__
#define __DISC_H__

#include "core/object.h"

struct Disc : Object {
  Disc(BSDF *mat) : Object(mat) { bounds = AABB(1); };
  bool hit(Ray& r, HitRec &rec);
  Vec sample(double *pdf, RNG& rng);
};

#endif // __DISC_H__