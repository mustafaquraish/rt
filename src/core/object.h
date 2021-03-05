#ifndef __OBJECT_H__
#define __OBJECT_H__

#include "core/ray.h"
#include "core/bsdf.h"
#include "core/primitive.h"
#include "core/aabb.h"

struct Object : Primitive {
  Object() {};
  Object(BSDF *bsdf): bsdf(bsdf) {};
  ~Object();

  // Public API for transforming objects
  void RotateX(double t);
  void RotateY(double t);
  void RotateZ(double t);
  void Scale(double x, double y, double z);
  void Scale(double s);
  void Translate(double x, double y, double z);
  
  void addNormalMap(Texture *tx);
  void addTextureMap(Texture *tx);
  void setMaterial(BSDF *_bsdf, bool override = false);

  virtual void finalize();

  // Internal utiliies
  Vec normalMapped(const Vec& canon_n, HitRec& rec);
  Ray rayTransform(const Ray& r);
  Vec normalTransform(const Vec& n);
  
  virtual BSDF *getBSDF(HitRec& rec) { return bsdf; }
  virtual bool hit(Ray& r, HitRec &rec) = 0;
  virtual Vec sample(double *pdf, RNG& rng);

  BSDF *bsdf = NULL;
  Texture *normalMap = NULL;
  double surfaceArea = 1;
  Primitive *prim = NULL;
  Matrix T, T_inv;
  int both = 0;
};

#endif // __OBJECT_H__
