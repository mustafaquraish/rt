#ifndef __BSDF_H__
#define __BSDF_H__

#include "core/math.h"

struct BSDFRec {
  Vec wo;      // Outgoing direction
  Vec wi;      // Incident direction (to be sampled)
  Vec n;       // Normal at the shading point
  double u, v; // Texture coordinates for intersection point
};

struct BSDF {
  BSDF(Colour col) : col(col) {}

  virtual Colour eval(BSDFRec& bRec) = 0;
  virtual Colour sample(BSDFRec& bRec) = 0;
  virtual double pdf(BSDFRec& bRec) = 0;
  virtual Colour emittance(BSDFRec& bRec) { return Vec(0); }

  bool isDiffuse() const { return diffuse; }
  bool isEmitter() const { return emitter; }
  bool isSpecular() const { return specular; }

  bool specular = false;
  bool diffuse = false;
  bool emitter = false;
  Colour col;
};

#endif // __BSDF_H__
