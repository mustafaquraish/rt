#include <materials/emitter.h>


Colour Emitter::eval(HitRec& rec) {
  return col(rec) * INV_PI;
}

Colour Emitter::sample(HitRec& rec, RNG& rng) {
  rec.wi = rng.randomVectorCosineHemisphere(rec.n);
  return col(rec);
}

double Emitter::pdf(HitRec& rec) {
  return dot(rec.wi, rec.n) * INV_PI;
}

Colour Emitter::emittance(HitRec& rec) {
  return col(rec);
}
