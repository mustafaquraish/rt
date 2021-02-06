#include "materials/transmissive.h"


Colour Transmissive::eval(BSDFRec& bRec) {
  return 0; // Delta BSDF
}


Colour Transmissive::reflect(BSDFRec& bRec) {
  Vec d = -bRec.wo, n = bRec.n;
  bRec.wi = d - 2 * dot(d, n) * n;
  return col;
}

Colour Transmissive::sample(BSDFRec& bRec) {  
  Vec d = -bRec.wo, n = bRec.n;
  double etaI = 1, etaT = ior;

  if (dot(d, n) > 0) {
    std::swap(etaI, etaT);
    n = -n;
  }

  double cosThetaI = dot(-n, norm(d));
  double eta = etaI / etaT;
  double disc = 1 - eta*eta * (1 - cosThetaI*cosThetaI);
  
  // Check for total internal reflection
  if (disc < 0) 
    return reflect(bRec);   // Total internal Reflection

  double cosThetaT = sqrt(disc);

  // Compute fresnel equations:
  double Rs = (etaI * cosThetaI - etaT * cosThetaT) /
              (etaI * cosThetaI + etaT * cosThetaT);
  double Rp = (etaT * cosThetaI - etaI * cosThetaT) /
              (etaT * cosThetaI + etaI * cosThetaT);
  
  // Percent of light to be reflected
  double reflectPct = (Rs * Rs + Rp * Rp) / 2.0;

  // Reflect with probability computed
  if (bRec.rng.rand01() < reflectPct) 
    return reflect(bRec);

  // Compute refracted direction
  bRec.wi = eta * norm(d) + (eta * cosThetaI - cosThetaT) * n;
  return col;
}

double Transmissive::pdf(BSDFRec& bRec) {
  return 0; // Delta BSDF
}
