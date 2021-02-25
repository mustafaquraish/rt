/**
 *                              PRIMITIVE
 *  A primitive an abstraction for all geometry in the scene that is 
 *  intersectable. It only requires a `hit()` function. All material
 *  information, transforms, etc are stored in the `Object` struct, which
 *  holds a primitive.
 */

#ifndef __PRIMITIVE_H__
#define __PRIMITIVE_H__

#include "core/ray.h"
#include "core/aabb.h"

// Forward declare
struct Object;

struct HitRec {
  double t;
  Vec p;
  Vec n;
  
  Vec wo;
  Vec wi;

  double u;
  double v;
  Object *obj;
};

struct Primitive {
  virtual ~Primitive() {};
  virtual bool hit(Ray& r, HitRec &rec) = 0;
  // Primitive bounds
  AABB bounds = AABB(Vec(-INFINITY), Vec(INFINITY));
};

struct Aggregate : Primitive {
  virtual bool hit(Ray& r, HitRec &rec) = 0;
};

#endif // __PRIMITIVE_H__
