#include <aggregates/primitive_list.h>

bool PrimitiveList::hit(Ray& ray, HitRec& rec) {
  bool hit = false;
  for (Primitive *prim : m_prims)
    if (prim->hit(ray, rec))
        hit = true;
  return hit;
}

PrimitiveList::~PrimitiveList() {
  for (auto prim : m_prims) {
    delete prim;
  }
}