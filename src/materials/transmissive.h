//
// Created by Mustafa Quraish on 2021-02-01.
//

#pragma once

#include <core/bsdf.h>

struct Transmissive : BSDF {
  Transmissive(float ior, Colour col) : BSDF(col), ior(ior) { }
  Transmissive(float ior, Texture *tx) : BSDF(tx), ior(ior) { }

  virtual bool isSpecular(HitRec &) const { return true; }

  Colour eval(HitRec& rec);
  Colour sample(HitRec& rec, RNG& rng);
  Colour reflect(HitRec& rec);
  float pdf(HitRec& rec);

  // Index of refraction
  float ior;
};

