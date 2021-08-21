#include <materials/glossy.h>
#include <core/reflection.h>
#include <core/rng.h>

Colour Glossy::eval(HitRec &rec) {
  return col(rec) * INV_PI;
}

void Glossy::initSample(HitRec &rec, RNG &rng) const {
  if (rng.rand01() <= m_roughness) {
    rec.rayType = HitRec::RayType::Diffuse;
  } else {
    rec.rayType = HitRec::RayType::Reflection;
  }
}

Colour Glossy::sample(HitRec &rec, RNG& rng) {
  if (rec.rayType == HitRec::RayType::Diffuse) {
    rec.wi = rng.randomVectorCosineHemisphere(rec.n);
    return col(rec);
  }

  rec.wi = Reflection::reflect(-rec.wo, rec.n);
  if (m_refl_sig > TOL)
    rec.wi = rng.randomVectorNormalDist(rec.wi, m_refl_sig);
  return col(rec);
}

float Glossy::pdf(HitRec &rec) {
  return dot(rec.wi, rec.n) * INV_PI;
}

bool Glossy::isDiffuse(HitRec &rec) const { 
  return rec.rayType == HitRec::RayType::Diffuse; 
}

bool Glossy::isSpecular(HitRec &rec) const { 
  return rec.rayType != HitRec::RayType::Diffuse; 
}