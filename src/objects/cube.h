#ifndef __CUBE_H__
#define __CUBE_H__

#include "core/object.h"

struct Cube : Object {
  Cube(Material *mat) : Object(mat) { bounds = AABB(1); };
  bool hit(Ray& r, HitRec &rec);
  Vec sample();
};

#endif // __CUBE_H__
