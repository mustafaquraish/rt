#ifndef __PLANE_H__
#define __PLANE_H__

#include "core/object.h"

struct Plane : Object {
Plane(BSDF *mat) : Object(mat) {
    bounds = AABB(Vec(-1,-1,0), Vec(1,1,0));
  };
  bool hit(Ray& r, HitRec &rec);
  Vec sample(double *pdf, RNG& rng);
};

#endif // __PLANE_H__
