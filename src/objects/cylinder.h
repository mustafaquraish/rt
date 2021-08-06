#pragma once

#include <core/object.h>

struct Cylinder : Object {
  Cylinder(BSDF *mat) : Object(mat) { bounds = AABB(1); };

  bool hitBase(double Z, Ray &r, HitRec &rec);
  bool hitSide(Ray &r, HitRec &rec);

  bool hit(Ray& r, HitRec &rec);
  Vec3 sample(double *pdf, RNG& rng);
};

