#pragma once

#include <core/ray.h>
#include <core/bsdf.h>
#include <core/primitive.h>
#include <core/aabb.h>

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
  Vec3 normalMapped(const Vec3& canon_n, HitRec& rec);
  Ray rayTransform(const Ray& r);
  Vec3 normalTransform(const Vec3& n);
  
  virtual BSDF *getBSDF(HitRec& rec) { return bsdf; }
  virtual bool hit(Ray& r, HitRec &rec) = 0;
  virtual Vec3 sample(double *pdf, RNG& rng);

  BSDF *bsdf = NULL;
  Texture *normalMap = NULL;
  double surfaceArea = 1;
  Primitive *prim = NULL;
  Matrix4 T, T_inv;
  int both = 0;
};

