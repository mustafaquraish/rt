#pragma once

#include <core/bsdf.h>

struct Emitter : BSDF {
  Emitter(Colour col) : BSDF(col) { }
  Emitter(Texture *tx) : BSDF(tx) { }
  
  virtual bool isEmitter() const { return true; }


  Colour eval(HitRec& rec);
  Colour sample(HitRec& rec, RNG& rng);
  float pdf(HitRec& rec);
  Colour emittance(HitRec& rec);
};

