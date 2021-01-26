#include "objects/sphere.h"
#include "core/rt.h"

// extern int DEBUG;

bool Sphere::hit(Ray& r, HitRec& rec) {

  int DEBUG = 0;

  Ray transformed = rayTransform(r);
  double A = dot(transformed.d, transformed.d);
  double B = dot(transformed.p, transformed.d) * 2;
  double C = dot(transformed.p, transformed.p) - 1;

  double t1, t2;

  if (!solveQuadratic(A, B, C, &t1, &t2)) {
    return false;
  }

  if (t1 < TOL && t2 < TOL) return false;

  if (t2 > TOL && (t2 < t1 || t1 < TOL)) {
    std::swap(t1, t2);
  }
  if (t1 < TOL) {
    return false;
  }

  rec.t1 = t1;
  rec.t2 = t2;
  rec.p = r.at(t1);
  rec.n = normalTransform(transformed.at(t1));
  rec.a = atan2(rec.p.z, rec.p.x) / (2 * PI) + 0.5;
  rec.b = 0.5 - asin(rec.p.y) / PI;
  rec.obj = this;
  return true;
}

Vec Sphere::sample() {
  return T * Vec(0,0,0);
}