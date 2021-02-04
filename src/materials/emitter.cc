#include "materials/emitter.h"


Colour Emitter::eval(BSDFRec& bRec) {
  return col * INV_PI;
}

Colour Emitter::sample(BSDFRec& bRec) {
  printf("Emitter::sample() unimplemented\n"); exit(1);
}

double Emitter::pdf(BSDFRec& bRec) {
  printf("Emitter::pdf() unimplemented\n"); exit(1);
}

Colour Emitter::emittance(BSDFRec& rec) {
  return col;
}
