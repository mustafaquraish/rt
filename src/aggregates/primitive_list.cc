#include "aggregates/primitive_list.h"

bool PrimitiveList::hit(Ray& ray, HitRec& rec) {
  double closest = INFINITY;
  HitRec temp;

  for (Primitive *prim : list) {
    // printf("--- %p\n", prim);
    if (prim->hit(ray, temp))
      if (temp.t1 > TOL && temp.t1 < closest)
        closest = temp.t1, rec = temp;
  }

  return (closest != INFINITY);
}
