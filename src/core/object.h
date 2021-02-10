#ifndef __OBJECT_H__
#define __OBJECT_H__

#include "core/ray.h"
#include "core/bsdf.h"
#include "core/primitive.h"
#include "core/aabb.h"

struct Object : Primitive {
  Object() {};
  Object(BSDF *bsdf): bsdf(bsdf) {};

  // Public API for transforming objects
  void RotateX(double t);
  void RotateY(double t);
  void RotateZ(double t);
  void Scale(double x, double y, double z);
  void Translate(double x, double y, double z);
  
  virtual void finalize();

  // Internal utiliies
  Ray rayTransform(const Ray& r);
  Vec normalTransform(const Vec& n);
  BSDF *getBSDF(HitRec& rec) { return bsdf; }
  
  virtual bool hit(Ray& r, HitRec &rec) = 0;
  virtual Vec sample(double *pdf, RNG& rng) = 0;

  BSDF *bsdf = NULL;
  double surfaceArea = 1;
  Primitive *prim = NULL;
  Matrix T, T_inv;
  int both = 0;
};

#endif // __OBJECT_H__
