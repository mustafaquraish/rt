#include "materials/lambertian.h"

Colour Lambertian::eval(BSDFRec &bRec) {
  return col * INV_PI;
}

Colour Lambertian::sample(BSDFRec &bRec) {
  bRec.wi = bRec.rng.randomVectorCosineHemisphere(bRec.n);
  return col;
}

double Lambertian::pdf(BSDFRec &bRec) {
  return dot(bRec.wi, bRec.n) * INV_PI;
}
