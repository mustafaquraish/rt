#include "objects/disc.h"
#include "core/rt.h"


bool Disc::hit(Ray& r, HitRec& rec) {
  Ray transformed = rayTransform(r);

  double t = -transformed.p.z / transformed.d.z;
  if (t < TOL || t > r.tMax) return false;

  Vec it = transformed.at(t);
  double xy_dist = it.x * it.x + it.y * it.y;
  if (fabs(xy_dist) > 1+TOL) return false;

  rec.p = r.at(t);
  rec.t = t;
  rec.n = normalTransform(Vec(0, 0, 1));
  rec.u = (it.x + 1) / 2;
  rec.v = (it.y + 1) / 2;
  rec.obj = this;
  r.tMax = min(r.tMax, rec.t);

  return true;
}

Vec Disc::sample(double *pdf, RNG& rng) {
  double x, y;
  do {
    x = 2 * rng.rand01() - 1;
    y = 2 * rng.rand01() - 1;
  } while (x*x + y*y > 1);

  *pdf = 1 / surfaceArea;
  return T * Vec(x, y, 0);
}