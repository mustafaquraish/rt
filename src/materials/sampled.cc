#include "materials/sampled.h"
#include "core/rng.h"
#include <assert.h>

SampledBSDF::SampledBSDF(const char *filename) {
  assert(MERL::read(m_brdf, filename));
}

Colour SampledBSDF::eval(HitRec &rec) {
  Vec3 wo = toLocalFrame(rec.wo, rec.n);
  Vec3 wi = toLocalFrame(rec.wi, rec.n);
 
//   auto inv = invert(get_rotation_matrix(rec.n));
//   Vec3 wo = inv * rec.wo;
//   Vec3 wi = inv * rec.wi;

  float theta_in, fi_in, theta_out, fi_out;
  theta_in = acosf(wi.z), fi_in = atan2f(wi.y, wi.x);
  theta_out = acosf(wo.z), fi_out = atan2f(wo.y, wo.x);

//   fi_in =  acosf(wi.z), theta_in = atan2f(wi.y, wi.x);
//   fi_out = acosf(wo.z), theta_out = atan2f(wo.y, wo.x);
//   printf("theta_in: %f, fi_in: %f, theta_out: %f, fi_out: %f\n", theta_in, fi_in, theta_out, fi_out);
//   printf("wo: %f, %f, %f\n", wo.x, wo.y, wo.z);
    // printf("wi: %f, %f, %f\n", wi.x, wi.y, wi.z);
  auto col =  MERL::lookup(m_brdf, theta_in, fi_in, theta_out, fi_out);
//   if (col.r < 0 || col.g < 0 || col.b < 0) {
//     printf("\ntheta_in: %f, fi_in: %f, theta_out: %f, fi_out: %f\n", theta_in, fi_in, theta_out, fi_out);
//     printf("wo: %f, %f, %f\n", wo.x, wo.y, wo.z);
//     printf("wi: %f, %f, %f\n", wi.x, wi.y, wi.z);
//     printf("col: %f, %f, %f\n", col.r, col.g, col.b);
//     exit(1);
//   }
  return sqrt(col);
//   return cmp_wise_min(col * 2, 1);
}

Colour SampledBSDF::sample(HitRec &rec, RNG &rng) {
  rec.wi = rng.randomVectorCosineHemisphere(rec.n);
  // FIXME: Should the dot product be here?
  return eval(rec) / pdf(rec);
}

float SampledBSDF::pdf(HitRec &rec) {
    return INV_PI;
}
