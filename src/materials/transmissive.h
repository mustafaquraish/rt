//
// Created by Mustafa Quraish on 2021-02-01.
//

#ifndef __TRANSMISSIVE_H__
#define __TRANSMISSIVE_H__

#include "core/bsdf.h"

struct Transmissive : BSDF {
  Transmissive(double ior, Colour col) : BSDF(col), ior(ior) { 
    specular = true; 
  }
  Colour eval(BSDFRec& bRec);
  Colour sample(BSDFRec& bRec);
  Colour reflect(BSDFRec& bRec);
  double pdf(BSDFRec& bRec);

  // Index of refraction
  double ior;
};

#endif //__TRANSMISSIVE_H__
