#include "materials/sampled.h"
#include "core/rng.h"
#include <assert.h>

SampledBSDF::SampledBSDF(const char *filename) {
  assert(MERL::read(m_brdf, filename));
}

Colour SampledBSDF::eval(HitRec &rec) {
  Vec3 wo = toLocalFrame(rec.wo, rec.n);
  Vec3 wi = toLocalFrame(rec.wi, rec.n);
  float theta_in, fi_in, theta_out, fi_out;
  theta_in = acosf(wi.z), fi_in = atan2f(wi.y, wi.x);
  theta_out = acosf(wo.z), fi_out = atan2f(wo.y, wo.x);
  return MERL::lookup(m_brdf, theta_in, fi_in, theta_out, fi_out);
}

Colour SampledBSDF::sample(HitRec &rec, RNG &rng) {
  rec.wi = rng.randomVectorCosineHemisphere(rec.n);
  // FIXME: Should the dot product be here?
  return eval(rec) * dot(rec.wi, rec.n) / pdf(rec);
}

float SampledBSDF::pdf(HitRec &rec) {
    return dot(rec.wi, rec.n) * INV_PI;
}
