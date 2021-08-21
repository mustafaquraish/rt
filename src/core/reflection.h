/**
 * This file does NOT contain any actual material definitions, but rather
 * some helpers that are useful for computing speecular reflection and
 * refraction.
 */
#pragma once

#include <core/rng.h>

namespace Reflection {

inline Vec3 reflect(const Vec3& d, const Vec3& n) {
  return d - 2 * dot(d, n) * n;
}

/**
 * Compute the refracted ray, with optional diffuse refraction.
 * 
 * NOTE: We need to pass an RNG instance here since there is a possible
 *       chance that the ray will be reflected instead.
 */
inline Vec3 refract(const Vec3& d_orig, const Vec3& n_orig, float ref_idx, RNG &rng) {
  
  Vec3 n = n_orig;
  Vec3 d = normalized(d_orig);
  float etaI = 1, etaT = ref_idx;
  float cosThetaI = -dot(n, d);

  if (cosThetaI < 0) {
    // float temp = etaI; etaI = etaT; etaT = temp;
    std::swap(etaI, etaT);
    n = -n;
    cosThetaI = -cosThetaI;
  }

  float eta = etaI / etaT;
  float disc = 1 - eta*eta * (1 - cosThetaI*cosThetaI);
  
  // Check for total internal reflection
  if (disc < 0) 
    return reflect(d, n);   // Total internal Reflection

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
    return reflect(d, n);

  // Compute refracted direction
  Vec3 res = eta * d + (eta * cosThetaI - cosThetaT) * n;
  return res;
}



}