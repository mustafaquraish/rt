#pragma once

#include <core/bsdf.h>

struct Glossy : BSDF {
  Glossy(Colour col, float pct=0.5) : BSDF(col), m_roughness(pct) {}
  Glossy(Texture *tx, float pct=0.5) : BSDF(tx), m_roughness(pct) {}

  virtual void initSample(HitRec &, RNG &) const;

  virtual bool isDiffuse(HitRec &) const;
  virtual bool isSpecular(HitRec &) const;

  Colour eval(HitRec& rec);
  Colour sample(HitRec& rec, RNG& rng);
  float pdf(HitRec& rec);

  float m_roughness;
};

