#ifndef __SPHERE_H__
#define __SPHERE_H__

#include "core/object.h"

struct Sphere : Object {
  Sphere(BSDF *mat) : Object(mat) { bounds = AABB(1); };
  bool hit(Ray& r, HitRec &rec);
  Vec sample(double *pdf, RNG& rng);
};

#endif // __SPHERE_H__