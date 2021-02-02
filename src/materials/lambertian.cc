//
// Created by Mustafa Quraish on 2021-02-01.
//

#include "lambertian.h"

Colour Lambertian::eval(BSDFRec &bRec) {
  return col * INV_PI;
}

Colour Lambertian::sample(BSDFRec &bRec) {
  bRec.wi = randomVectorCosineHemisphere(bRec.n);
  return col;
}

double Lambertian::pdf(BSDFRec &bRec) {
  return dot(bRec.wi, bRec.n) * INV_PI;
}
