#pragma once

#include <core/primitive.h>
#include <core/object.h>

struct PrimitiveList : Aggregate {
  PrimitiveList(std::vector<Primitive *>& list) {
    m_prims = list;
    bounds = AABB();
    for (Primitive *prim : m_prims) 
      bounds = combine(bounds, prim->bounds);
  };
  ~PrimitiveList();
  bool hit(Ray& r, HitRec &rec);
};

