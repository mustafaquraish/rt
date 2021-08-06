//
// Created by Mustafa Quraish on 2021-02-01.
//

#pragma once

#include <core/bsdf.h>

struct OrenNayar : BSDF {
  OrenNayar(double roughness, Colour col);
  Colour eval(HitRec& rec);
  Colour sample(HitRec& rec, RNG& rng);
  double pdf(HitRec& rec);

  double A;
  double B;
};
