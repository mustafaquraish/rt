#include <materials/specular.h>

#include <core/rng.h>

namespace Specular {

__attribute__((always_inline))
Vec3 reflect(const Vec3& d, const Vec3& n, RNG &rng, float refl_sig) {
  Vec3 res = d - 2 * dot(d, n) * n;
  if (refl_sig > TOL)
    res = rng.randomVectorNormalDist(res, refl_sig);
  return res;
}

__attribute__((always_inline))
Vec3 refract(const Vec3& d, const Vec3& n_orig, float ref_idx, RNG &rng, float refl_sig) {
  
  Vec3 n = n_orig;
  float etaI = 1, etaT = ref_idx;

  if (dot(d, n) > 0) {
    std::swap(etaI, etaT);
    n = -n;
  }

  float cosThetaI = dot(-n, normalized(d));
  float eta = etaI / etaT;
  float disc = 1 - eta*eta * (1 - cosThetaI*cosThetaI);
  
  // Check for total internal reflection
  if (disc < 0) 
    return reflect(d, n, rng, refl_sig);   // Total internal Reflection

  float cosThetaT = sqrt(disc);

  // Compute fresnel equations:
  float Rs = (etaI * cosThetaI - etaT * cosThetaT)
           / (etaI * cosThetaI + etaT * cosThetaT);
  float Rp = (etaT * cosThetaI - etaI * cosThetaT)
           / (etaT * cosThetaI + etaI * cosThetaT);
  
  // Percent of light to be reflected
  float reflectPct = (Rs * Rs + Rp * Rp) / 2.0;

  // Reflect with probability computed
  if (rng.rand01() < reflectPct) 
    return reflect(d, n, rng, refl_sig);

  // Compute refracted direction
  Vec3 res = eta * normalized(d) + (eta * cosThetaI - cosThetaT) * n;
  if (refl_sig > TOL)
    res = rng.randomVectorNormalDist(res, refl_sig);
  return res;
}



}