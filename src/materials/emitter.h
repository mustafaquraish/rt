#pragma once

#include <core/bsdf.h>

struct Emitter : BSDF {
  Emitter(Colour col) : BSDF(col) { emitter = true; }
  Emitter(Texture *tx) : BSDF(tx) { emitter = true; }
  
  Colour eval(HitRec& rec);
  Colour sample(HitRec& rec, RNG& rng);
  double pdf(HitRec& rec);
  Colour emittance(HitRec& rec);
};

