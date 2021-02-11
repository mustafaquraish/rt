//
// Created by Mustafa Quraish on 2021-02-01.
//

#ifndef __OREN_NAYAR_H__
#define __OREN_NAYAR_H__

#include "core/bsdf.h"

struct OrenNayar : BSDF {
  OrenNayar(double roughness, Colour col);
  Colour eval(BSDFRec& bRec);
  Colour sample(BSDFRec& bRec);
  double pdf(BSDFRec& bRec);

  double A;
  double B;
};

#endif //__OREN_NAYAR_H__
