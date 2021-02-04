#include "materials/transmissive.h"


Colour Transmissive::eval(BSDFRec& bRec) {
  return 0; // Delta BSDF
}

Colour Transmissive::sample(BSDFRec& bRec) {
  bool reflect = false;
  
  Vec d = -bRec.wo, n = bRec.n;
  double r = 1 / ior;

  if (dot(d, n) > 0) {
    r = ior;
    n = -n;
  }

  double c = dot(-n, norm(d));
  double disc = 1 - r*r * (1 - c*c);
  if (disc < TOL) return 0;   // Total internal Reflection

  bRec.wi = r * norm(d) + (r * c - sqrt(disc)) * n;
  return col;
}

double Transmissive::pdf(BSDFRec& bRec) {
  return 0; // Delta BSDF
}
