#ifndef __LAMBERTIAN_H__
#define __LAMBERTIAN_H__

#include "core/bsdf.h"

struct Lambertian : BSDF {
  Lambertian(Colour col) : BSDF(col) { diffuse = true; }
  Lambertian(Texture *tx) : BSDF(tx) { diffuse = true; }
  Colour eval(HitRec& rec);
  Colour sample(HitRec& rec, RNG& rng);
  double pdf(HitRec& rec);
};

#endif //__LAMBERTIAN_H__
