#include "materials/emitter.h"


Colour Emitter::eval(BSDFRec& bRec) {
  return col * INV_PI;
}

Colour Emitter::sample(BSDFRec& bRec) {
  bRec.wi = bRec.rng.randomVectorCosineHemisphere(bRec.n);
  return col;
}

double Emitter::pdf(BSDFRec& bRec) {
  return dot(bRec.wi, bRec.n) * INV_PI;
}

Colour Emitter::emittance(BSDFRec& rec) {
  return col;
}
