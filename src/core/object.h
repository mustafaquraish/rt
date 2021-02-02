#ifndef __OBJECT_H__
#define __OBJECT_H__

#include "core/ray.h"
#include "core/material.h"
#include "materials/lambertian.h"
#include "core/primitive.h"
#include "core/aabb.h"

struct Object : Primitive {
  Object() {};
  Object(Material *mat): mat(mat), bsdf(new Lambertian(mat->col)) {};

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
  Material *getMaterial(HitRec& rec) { return mat; }
  
  virtual bool hit(Ray& r, HitRec &rec) = 0;
  virtual Vec sample() = 0;

  Material *mat = NULL;
  BSDF *bsdf;
  Primitive *prim = NULL;
  Matrix T, T_inv;
  int both = 0;
};

#endif // __OBJECT_H__
