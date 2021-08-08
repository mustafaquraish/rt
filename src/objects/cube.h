#pragma once

#include "core/object.h"

struct Cube : Object {
  Cube(BSDF *mat) : Object(mat) { bounds = AABB(1); };
  bool hit(Ray& r, HitRec &rec);
  Vec3 sample(float *pdf, RNG& rng);
};

