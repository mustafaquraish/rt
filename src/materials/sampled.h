#pragma once

#include <ext/MERL.h>
#include <core/bsdf.h>

struct SampledBSDF : BSDF {
  SampledBSDF(const char *filename);

  bool isDiffuse(HitRec &) const { return true; };
  bool isSpecular(HitRec &) const { return false; };

  Colour eval(HitRec& rec);
  Colour sample(HitRec& rec, RNG& rng);
  float pdf(HitRec& rec);

  MERL::BRDF m_brdf;
};
