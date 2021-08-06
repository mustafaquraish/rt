//
// Created by Mustafa Quraish on 2021-02-01.
//
#pragma once

#include <core/bsdf.h>

struct Mirror : BSDF {
  Mirror(Colour col) : BSDF(col) { specular = true; }
  Mirror(Texture *tx) : BSDF(tx) { specular = true; }
  Colour eval(HitRec& rec);
  Colour sample(HitRec& rec, RNG& rng);
  double pdf(HitRec& rec);
};
