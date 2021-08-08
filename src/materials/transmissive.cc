#include <materials/transmissive.h>


Colour Transmissive::eval(HitRec& rec) {
  return 0; // Delta BSDF
}


Colour Transmissive::reflect(HitRec& rec) {
  Vec3 d = -rec.wo, n = rec.n;
  rec.wi = d - 2 * dot(d, n) * n;
  return col(rec);
}

Colour Transmissive::sample(HitRec& rec, RNG& rng) {  
  Vec3 d = -rec.wo, n = rec.n;
  float etaI = 1, etaT = ior;

  if (dot(d, n) > 0) {
    std::swap(etaI, etaT);
    n = -n;
  }

  float cosThetaI = dot(-n, normalized(d));
  float eta = etaI / etaT;
  float disc = 1 - eta*eta * (1 - cosThetaI*cosThetaI);
  
  // Check for total internal reflection
  if (disc < 0) 
    return reflect(rec);   // Total internal Reflection

  float cosThetaT = sqrt(disc);

  // Compute fresnel equations:
  float Rs = (etaI * cosThetaI - etaT * cosThetaT) /
              (etaI * cosThetaI + etaT * cosThetaT);
  float Rp = (etaT * cosThetaI - etaI * cosThetaT) /
              (etaT * cosThetaI + etaI * cosThetaT);
  
  // Percent of light to be reflected
  float reflectPct = (Rs * Rs + Rp * Rp) / 2.0;

  // Reflect with probability computed
  if (rng.rand01() < reflectPct) 
    return reflect(rec);

  // Compute refracted direction
  rec.wi = eta * normalized(d) + (eta * cosThetaI - cosThetaT) * n;
  return col(rec);
}

float Transmissive::pdf(HitRec& rec) {
  return 0; // Delta BSDF
}
