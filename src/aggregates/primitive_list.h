#pragma once

#include <core/primitive.h>
#include <core/object.h>
#include <vector>

struct PrimitiveList : Aggregate {
  PrimitiveList(std::vector<Primitive *>& list) : list(list) {
    bounds = AABB();
    for (Primitive *prim : list) 
      bounds = combine(bounds, prim->bounds);
  };
  ~PrimitiveList();
  bool hit(Ray& r, HitRec &rec);
  std::vector<Primitive *> list;
};

