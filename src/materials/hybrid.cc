#include <materials/hybrid.h>
#include <core/reflection.h>

Hybrid::Hybrid(Colour col, float diff, float refl, float refr, float refl_sig, float refr_idx)
  : BSDF(col), m_diff(diff), m_refl(refl), m_refr(refr), m_refl_sig(refl_sig), m_ior(refr_idx) {}

Hybrid::Hybrid(Texture *tx, float diff, float refl, float refr, float refl_sig, float refr_idx)
  : BSDF(tx), m_diff(diff), m_refl(refl), m_refr(refr), m_refl_sig(refl_sig), m_ior(refr_idx) {}

Colour Hybrid::eval(HitRec &rec) {
  return col(rec) * INV_PI;
}

void Hybrid::initSample(HitRec &rec, RNG &rng) const {
  float dice = rng.rand01();
  if (dice <= m_diff)
    rec.rayType = HitRec::RayType::Diffuse;
  else if (dice <= m_diff + m_refl)
    rec.rayType = HitRec::RayType::Reflection;
  else
    rec.rayType = HitRec::RayType::Refraction;
}

Colour Hybrid::sample(HitRec &rec, RNG& rng) {
  if (rec.rayType == HitRec::RayType::Diffuse) {
    rec.wi = rng.randomVectorCosineHemisphere(rec.n);
    return col(rec);
  } 
  
  if (rec.rayType == HitRec::RayType::Reflection)
    rec.wi = Reflection::reflect(-rec.wo, rec.n);
  else
    rec.wi = Reflection::refract(-rec.wo, rec.n, m_ior, rng);
  
  if (m_refl_sig > TOL)
    rec.wi = rng.randomVectorNormalDist(rec.wi, m_refl_sig);

  return col(rec);
}

float Hybrid::pdf(HitRec &rec) {
  return dot(rec.wi, rec.n) * INV_PI;
}

bool Hybrid::isDiffuse(HitRec &rec) const { 
  return rec.rayType == HitRec::RayType::Diffuse; 
}

bool Hybrid::isSpecular(HitRec &rec) const { 
  return rec.rayType != HitRec::RayType::Diffuse; 
}