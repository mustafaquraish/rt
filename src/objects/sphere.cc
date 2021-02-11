#include "objects/sphere.h"
#include "core/rt.h"

extern int DEBUG;

bool Sphere::hit(Ray& r, HitRec& rec) {
  Ray transformed = rayTransform(r);
  double A = dot(transformed.d, transformed.d);
  double B = dot(transformed.p, transformed.d) * 2;
  double C = dot(transformed.p, transformed.p) - 1;

  double t1, t2;

  if (!solveQuadratic(A, B, C, &t1, &t2)) {
    return false;
  }

  if (t1 < TOL && t2 < TOL) return false;
  if (t2 > TOL && (t2 < t1 || t1 < TOL)) t1 = t2;
  if (t1 > r.tMax) return false;

  rec.t = t1;
  rec.p = r.at(t1);
  rec.n = normalTransform(transformed.at(t1));
  rec.u = atan2(rec.p.z, rec.p.x) / (2 * PI) + 0.5;
  rec.v = 0.5 - asin(rec.p.y) / PI;
  rec.obj = this;
  r.tMax = min(r.tMax, rec.t);
  return true;
}

Vec Sphere::sample(double *pdf, RNG& rng) {
  double theta = 2 * PI * rng.rand01();
  double phi = acos(2 * rng.rand01() - 1);
  
  *pdf = 1.0 / surfaceArea;
  
  return T * Vec(cos(theta) * sin(phi),
                 sin(theta) * sin(phi),
                 cos(phi));
}