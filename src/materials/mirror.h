//
// Created by Mustafa Quraish on 2021-02-01.
//

#ifndef __MIRROR_H__
#define __MIRROR_H__

#include "core/bsdf.h"

struct Mirror : BSDF {
  Mirror(Colour col) : BSDF(col) { specular = true; }
  Colour eval(BSDFRec& bRec);
  Colour sample(BSDFRec& bRec);
  double pdf(BSDFRec& bRec);
};

#endif //__MIRROR_H__
