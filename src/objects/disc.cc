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
  rec.a = (it.x + 1) / 2;
  rec.b = (it.y + 1) / 2;
  rec.obj = this;
  r.tMax = min(r.tMax, rec.t);

  return true;
}

Vec Disc::sample() {
  return T * Vec(0,0,0);
}