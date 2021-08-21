#pragma once

#include <materials/basic.h>

struct Emitter : Lambertian {
  using Lambertian::Lambertian;
  virtual bool isEmitter() const { return true; }
  Colour emittance(HitRec& rec) { return col(rec); };
};

