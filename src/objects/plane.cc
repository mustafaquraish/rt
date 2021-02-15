#include "objects/plane.h"

bool Plane::hit(Ray& r, HitRec& rec) {
  Ray transformed = rayTransform(r);
  double t = -transformed.p.z / transformed.d.z;
  Vec it = transformed.at(t);
  if (t < TOL || t > r.tMax || 
      fabs(it.x) > 1 || fabs(it.y) > 1) {
    return false;
  } else {
    rec.t = t;
    rec.p = r.at(t);
    rec.u = (it.x + 1) / 2.0;
    rec.v = (it.y + 1) / 2.0;
    Vec canon_n = Vec(0, 0, 1);
    canon_n = normalMapped(canon_n, rec);
    rec.n = normalTransform(canon_n);
    if (dot(r.d, rec.n) > 0) rec.n = -rec.n;
    rec.obj = this;
    r.tMax = min(r.tMax, t);
    return true;
  }
}

Vec Plane::sample(double *pdf, RNG& rng) {
  double x = rng.rand01() * 2 - 1;
  double y = rng.rand01() * 2 - 1;
  *pdf = 1 / surfaceArea;
  return T * Vec(x, y, 0);
}

