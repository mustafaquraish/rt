#include <materials/basic.h>
#include <core/reflection.h>

Colour Lambertian::eval(HitRec &rec) { 
  return col(rec) * INV_PI; 
}

Colour Lambertian::sample(HitRec &rec, RNG& rng) {
  rec.wi = rng.randomVectorCosineHemisphere(rec.n);
  return col(rec);
}

float Lambertian::pdf(HitRec &rec) {
  return dot(rec.wi, rec.n) * INV_PI;
}

Colour Mirror::sample(HitRec& rec, RNG& rng) {
  rec.wi = Reflection::reflect(-rec.wo, rec.n);
  return col(rec);
}

Colour Transmissive::sample(HitRec& rec, RNG& rng) {  
  rec.wi = Reflection::refract(-rec.wo, rec.n, m_ior, rng);
  return col(rec);
}
