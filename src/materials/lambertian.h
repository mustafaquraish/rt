//
// Created by Mustafa Quraish on 2021-02-01.
//

#ifndef __LAMBERTIAN_H__
#define __LAMBERTIAN_H__

#include "core/bsdf.h"

struct Lambertian : BSDF {
  Lambertian(Colour col) : BSDF(col) { diffuse = true; }
  Colour eval(BSDFRec& bRec);
  Colour sample(BSDFRec& bRec);
  double pdf(BSDFRec& bRec);
};

#endif //__LAMBERTIAN_H__
