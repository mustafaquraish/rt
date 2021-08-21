/**
 * Contains the simplest perfect materials:
 *  - Lambertian
 *  - Mirror
 *  - Transmissive
 */

#pragma once

#include <core/bsdf.h>

struct Lambertian : BSDF {
  Lambertian(Colour col) : BSDF(col) { }
  Lambertian(Texture *tx) : BSDF(tx) { }
  virtual bool isDiffuse(HitRec &) const { return true; }

  Colour eval(HitRec& rec);
  Colour sample(HitRec& rec, RNG& rng);
  float pdf(HitRec& rec);
};

struct Mirror : BSDF {
  Mirror(Colour col) : BSDF(col) { }
  Mirror(Texture *tx) : BSDF(tx) { }
  virtual bool isSpecular(HitRec &) const { return true; }
  
  Colour sample(HitRec& rec, RNG& rng);
};

struct Transmissive : BSDF {
  Transmissive(float ior, Colour col) : BSDF(col), m_ior(ior) { }
  Transmissive(float ior, Texture *tx) : BSDF(tx), m_ior(ior) { }

  virtual bool isSpecular(HitRec &) const { return true; }

  Colour sample(HitRec& rec, RNG& rng);
  
  // Index of refraction
  float m_ior;
};
