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
    rec.n = normalTransform(Vec(0,0,1));
    rec.u = it.x*2 - 1;
    rec.v = it.y*2 - 1;
    rec.obj = this;
    r.tMax = min(r.tMax, t);
    return true;
  }
}

Vec Plane::sample() {
  Vec p = Vec(rand01(), rand01(), 0.5) * 2 - 1;
  return T * p;
}

