#ifndef __SPHERE_H__
#define __SPHERE_H__

#include "core/object.h"

struct Sphere : Object {
  Sphere(Material *mat) : Object(mat) { bounds = AABB(1); };
  bool hit(Ray& r, HitRec &rec);
  Vec sample();
};

#endif // __SPHERE_H__