#pragma once

#include <core/bsdf.h>

struct Glossy : BSDF {
  Glossy(Colour col, float pct=0.5, float refl_sig=0) 
    : BSDF(col), m_roughness(pct), m_refl_sig(refl_sig) {}
  Glossy(Texture *tx, float pct=0.5, float refl_sig=0) 
    : BSDF(tx), m_roughness(pct), m_refl_sig(refl_sig) {}

  virtual void initSample(HitRec &, RNG &) const;

  virtual bool isDiffuse(HitRec &) const;
  virtual bool isSpecular(HitRec &) const;

  Colour eval(HitRec& rec);
  Colour sample(HitRec& rec, RNG& rng);
  float pdf(HitRec& rec);

  float m_roughness;
  float m_refl_sig;
};

