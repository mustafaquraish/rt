#include "objects/sdf.h"

#define SDF_MAX_ITERS 1000
#define SDF_EPS 10e-4

bool SDFObject::hit(Ray& r, HitRec& rec) {
  Ray transformed = rayTransform(r);

  double lambda = SDF_EPS, dist;
  Vec p;
  int iter = 0;

  while (iter++ < SDF_MAX_ITERS) {
    p = transformed.at(lambda);
    dist = F(p);
    lambda += dist;
    if (fabs(dist) < SDF_EPS)
      break;
  }

  if (iter > SDF_MAX_ITERS)
    return false;

  Vec canon_n = (Vec(F(p + Vec(EPS,  0,  0)),
                     F(p + Vec(  0,EPS,  0)),
                     F(p + Vec(  0,  0,EPS))) - F(p)) / EPS;
  canon_n = norm(canon_n);

  rec.t = lambda;
  rec.p = r.at(lambda);

  rec.u = atan2(canon_n.z, canon_n.x) / (2 * PI) + 0.5;
  rec.v = 0.5 - asin(canon_n.y) / PI;

  canon_n = normalMapped(canon_n, rec);
  rec.n = normalTransform(canon_n);

  rec.obj = this;
  rec.stepsRatio = (double) iter / (double) SDF_MAX_ITERS;
  r.tMax = min(r.tMax, lambda);

  return true;
}

#undef SDF_MAX_ITERS
#undef SDF_EPS
