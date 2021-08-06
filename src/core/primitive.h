/**
 *                              PRIMITIVE
 *  A primitive an abstraction for all geometry in the scene that is 
 *  intersectable. It only requires a `hit()` function. All material
 *  information, transforms, etc are stored in the `Object` struct, which
 *  holds a primitive.
 */

#pragma once

#include <core/ray.h>
#include <core/aabb.h>

// Forward declare
struct Object;

struct HitRec {
  float t;
  Vec3 p;
  Vec3 n;
  
  Vec3 wo;
  Vec3 wi;

  float u;
  float v;
  Object *obj;

  float stepsRatio; // For SDFs
};

struct Primitive {
  virtual ~Primitive() {};
  virtual bool hit(Ray& r, HitRec &rec) = 0;
  // Primitive bounds
  AABB bounds = AABB(Vec3(-INFINITY), Vec3(INFINITY));
};

struct Aggregate : Primitive {
  virtual bool hit(Ray& r, HitRec &rec) = 0;
};

