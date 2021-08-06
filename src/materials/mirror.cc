#include <materials/mirror.h>


Colour Mirror::eval(HitRec& rec) {
  return 0; // Delta BSDF    
}

Colour Mirror::sample(HitRec& rec, RNG& rng) {
  Vec3 d = -rec.wo, n = rec.n;
  rec.wi = d - 2 * dot(d, n) * n;
  return col(rec);
}

float Mirror::pdf(HitRec& rec) {
  return 0; // Delta BSDF
}
