#ifndef __CYLINDER_H__
#define __CYLINDER_H__

#include "core/object.h"

struct Cylinder : Object {
  Cylinder(BSDF *mat) : Object(mat) { bounds = AABB(1); };
  bool hit(Ray& r, HitRec &rec);
  Vec sample(double *pdf, RNG& rng);
};

#endif // __CYLINDER_H__