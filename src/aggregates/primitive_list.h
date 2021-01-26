#ifndef __PRIMITIVE_LIST_H__
#define __PRIMITIVE_LIST_H__

#include "core/primitive.h"
#include "core/object.h"
#include <vector>

struct PrimitiveList : Aggregate {
  PrimitiveList(std::vector<Primitive *>& list) : list(list) {
    bounds = list[0]->bounds;
    for (Primitive *prim : list) 
      bounds = combine(bounds, prim->bounds);
  };
  bool hit(Ray& r, HitRec &rec);
  std::vector<Primitive *> list;
};

#endif // __PRIMITIVE_LIST_H__
