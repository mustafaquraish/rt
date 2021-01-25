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
  double t1;
  double t2;
  Vec p;
  Vec n;
  double a;
  double b;
  Object *obj;
};

struct Primitive {
  virtual bool hit(Ray& r, HitRec &rec) = 0;
  // Get primitive bounds
  virtual AABB getBounds() const = 0;
};

struct Aggregate : Primitive {
  virtual bool hit(Ray& r, HitRec &rec) = 0;
};

#endif // __PRIMITIVE_H__
