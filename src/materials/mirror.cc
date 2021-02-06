#include "materials/mirror.h"


Colour Mirror::eval(BSDFRec& bRec) {
  return 0; // Delta BSDF    
}

Colour Mirror::sample(BSDFRec& bRec) {
  Vec d = -bRec.wo, n = bRec.n;
  bRec.wi = d - 2 * dot(d, n) * n;
  return col;
}

double Mirror::pdf(BSDFRec& bRec) {
  return 0; // Delta BSDF
}
