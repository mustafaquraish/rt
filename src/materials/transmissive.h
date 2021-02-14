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
  Transmissive(double ior, Texture *tx) : BSDF(tx), ior(ior) { 
    specular = true; 
  }
  Colour eval(HitRec& rec);
  Colour sample(HitRec& rec, RNG& rng);
  Colour reflect(HitRec& rec);
  double pdf(HitRec& rec);

  // Index of refraction
  double ior;
};

#endif //__TRANSMISSIVE_H__
