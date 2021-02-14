//
// Created by Mustafa Quraish on 2021-02-01.
//

#ifndef __OREN_NAYAR_H__
#define __OREN_NAYAR_H__

#include "core/bsdf.h"

struct OrenNayar : BSDF {
  OrenNayar(double roughness, Colour col);
  Colour eval(HitRec& rec);
  Colour sample(HitRec& rec, RNG& rng);
  double pdf(HitRec& rec);

  double A;
  double B;
};

#endif //__OREN_NAYAR_H__
