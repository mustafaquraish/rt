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

