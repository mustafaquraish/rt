/**
 * This is the "hybrid" texture which is mostly for convenience,
 * and not physically based at all. It provides a convenient way
 * to have a material which is reflective/refractive/diffuse
 */

#pragma once

#include <core/bsdf.h>

struct Hybrid : BSDF {
  Hybrid(Colour col, float diff, float refl, float refr, float refl_sig=0, float refr_idx=1.47);
  Hybrid(Texture *tx, float diff, float refl, float refr, float refl_sig=0, float refr_idx=1.47);

  virtual void initSample(HitRec &, RNG &) const;

  virtual bool isDiffuse(HitRec &) const;
  virtual bool isSpecular(HitRec &) const;

  Colour eval(HitRec& rec);
  Colour sample(HitRec& rec, RNG& rng);
  float pdf(HitRec& rec);

  // Percent of time we choose a diffuse ray
  float m_diff;
  
  // Percent of time we pick a reflection ray
  float m_refl;
  
  // Percent of time we pick a refraction ray
  float m_refr;
  float m_ior;

  // Possible sigma for perturbing the specular bounces
  float m_refl_sig;
    
  void sampleDiffuse(HitRec& rec, RNG& rng);
  void sampleMirror(HitRec& rec, RNG& rng);
  void sampleRefract(HitRec& rec, RNG& rng);
};

