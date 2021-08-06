#pragma once

#include <core/object.h>

struct Plane : Object {
Plane(BSDF *mat) : Object(mat) {
    bounds = AABB(Vec3(-1, -1, 0), Vec3(1, 1, 0));
  };
  bool hit(Ray& r, HitRec &rec);
  Vec3 sample(double *pdf, RNG& rng);
};

