#ifndef __DISC_H__
#define __DISC_H__

#include "core/object.h"

struct Disc : Object {
  Disc(Material *mat) : Object(mat) { bounds = AABB(1); };
  bool hit(Ray& r, HitRec &rec);
  Vec sample();
};

#endif // __DISC_H__