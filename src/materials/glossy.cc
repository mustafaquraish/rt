#include <materials/glossy.h>
#include <materials/lambertian.h>
#include <materials/mirror.h>
#include <core/rng.h>

Colour Glossy::eval(HitRec &rec) {
  return col(rec) * INV_PI;
}

void Glossy::initSample(HitRec &rec, RNG &rng) const {
  if (rng.rand01() <= m_roughness) {
    rec.rayType = HitRec::RayType::Diffuse;
  } else {
    rec.rayType = HitRec::RayType::Specular;
  }
}

Colour Glossy::sample(HitRec &rec, RNG& rng) {
  if (rec.rayType == HitRec::RayType::Diffuse) {
    rec.wi = rng.randomVectorCosineHemisphere(rec.n);
  } else {
    Vec3 d = -rec.wo, n = rec.n;
    rec.wi = d - 2 * dot(d, n) * n;
    if (m_refl_sig > 0) {
      rec.wi = rng.randomVectorNormalDist(rec.wi, m_refl_sig);
    }
  }
  return col(rec);
}

float Glossy::pdf(HitRec &rec) {
  return dot(rec.wi, rec.n) * INV_PI;
}

bool Glossy::isDiffuse(HitRec &rec) const { 
  return rec.rayType == HitRec::RayType::Diffuse; 
}

bool Glossy::isSpecular(HitRec &rec) const { 
  return rec.rayType == HitRec::RayType::Specular; 
}