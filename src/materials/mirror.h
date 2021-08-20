//
// Created by Mustafa Quraish on 2021-02-01.
//
#pragma once

#include <core/bsdf.h>

struct Mirror : BSDF {
  Mirror(Colour col) : BSDF(col) { }
  Mirror(Texture *tx) : BSDF(tx) { }

  virtual bool isSpecular(HitRec &) const { return true; }

  Colour eval(HitRec& rec);
  Colour sample(HitRec& rec, RNG& rng);
  float pdf(HitRec& rec);
};
