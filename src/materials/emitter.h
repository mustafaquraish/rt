#ifndef __EMITTER_H__
#define __EMITTER_H__

#include "core/bsdf.h"

struct Emitter : BSDF {
  Emitter(Colour col) : BSDF(col) { emitter = true; }
  
  Colour eval(HitRec& rec);
  Colour sample(HitRec& rec, RNG& rng);
  double pdf(HitRec& rec);
  Colour emittance(HitRec& rec);
};

#endif // __EMITTER_H__
