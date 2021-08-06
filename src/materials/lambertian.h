#pragma once

#include <core/bsdf.h>

struct Lambertian : BSDF {
  Lambertian(Colour col) : BSDF(col) { diffuse = true; }
  Lambertian(Texture *tx) : BSDF(tx) { diffuse = true; }
  Colour eval(HitRec& rec);
  Colour sample(HitRec& rec, RNG& rng);
  double pdf(HitRec& rec);
};

