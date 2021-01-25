#include "objects/plane.h"

bool Plane::hit(Ray& r, HitRec& rec) {
  Ray transformed = rayTransform(r);
  double lambda = -transformed.p.z / transformed.d.z;
  Vec it = transformed.at(lambda);
  if (lambda < TOL || fabs(it.x) > 1 || fabs(it.y) > 1) {
    return false;
  } else {
    rec.t1 = lambda;
    rec.t2 = lambda;
    rec.p = r.at(lambda);
    rec.n = normalTransform(Vec(0,0,1));
    rec.a = it.x*2 - 1;
    rec.b = it.y*2 - 1;
    rec.obj = this;
    return true;
  }
}

Vec Plane::sample() {
  Vec p = Vec(rand01(), rand01(), 0.5) * 2 - 1;
  return T * p;
}

