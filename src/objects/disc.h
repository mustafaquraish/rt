#pragma once

#include <core/object.h>

struct Disc : Object {
  Disc(BSDF *mat) : Object(mat) { bounds = AABB(1); };
  bool hit(Ray& r, HitRec &rec);
  Vec3 sample(double *pdf, RNG& rng);
};

