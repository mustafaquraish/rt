#include "materials/lambertian.h"

Colour Lambertian::eval(HitRec &rec) {
  return col(rec) * INV_PI;
}

Colour Lambertian::sample(HitRec &rec, RNG& rng) {
  rec.wi = rng.randomVectorCosineHemisphere(rec.n);
  return col(rec);
}

double Lambertian::pdf(HitRec &rec) {
  return dot(rec.wi, rec.n) * INV_PI;
}
