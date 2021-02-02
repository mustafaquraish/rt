#ifndef __EMITTER_H__
#define __EMITTER_H__

#include "core/bsdf.h"

struct Emitter : BSDF {
  Emitter(Colour col) : BSDF(col) { emitter = true; }
  Colour eval(BSDFRec& bRec);
  Colour sample(BSDFRec& bRec);
  double pdf(BSDFRec& bRec);
  Colour emittance(BSDFRec& rec);
};

#endif // __EMITTER_H__
