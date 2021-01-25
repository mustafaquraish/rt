#ifndef __PRIMITIVE_LIST_H__
#define __PRIMITIVE_LIST_H__

#include "core/primitive.h"
#include "core/object.h"
#include <vector>

struct PrimitiveList : Aggregate {
  PrimitiveList(std::vector<Primitive *>& list) : list(list) {};
  bool hit(Ray& r, HitRec &rec);
  std::vector<Primitive *> list;
  // This should never really be used
  AABB getBounds() const;
};

#endif // __PRIMITIVE_LIST_H__
