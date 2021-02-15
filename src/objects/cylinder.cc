#include "objects/cylinder.h"
#include "core/rt.h"

// extern int DEBUG;

bool Cylinder::hitBase(double Z, Ray &r, HitRec &rec) {
  double t =  (Z - r.p.z) / r.d.z;
  if (t < TOL || t > r.tMax) return false;
  Vec it = r.at(t);
  double xy_dist = it.x * it.x + it.y * it.y;
  if (fabs(xy_dist) > 1+TOL) return false;
  rec.t = t;
  rec.u = (it.x + 1) / 2;
  rec.v = (it.y + 1) / 2;
  rec.n = normalMapped(Vec(0, 0, Z), rec);
  r.tMax = min(r.tMax, rec.t);
  return true;
}

bool Cylinder::hitSide(Ray &r, HitRec &rec) {
  double A = r.d.x * r.d.x + r.d.y * r.d.y;
  double B = (r.p.x * r.d.x + r.p.y * r.d.y) * 2;
  double C = (r.p.x * r.p.x + r.p.y * r.p.y) - 1;

  double t1, t2;

  if (!solveQuadratic(A, B, C, &t1, &t2)) return false;
  if (t1 < TOL && t2 < TOL) return false;
  if (t2 > TOL && (t2 < t1 || t1 < TOL)) t1 = t2;
  if (t1 < TOL || t1 > r.tMax) return false;

  Vec it = r.at(t1);
  if (fabs(it.z) > 1+TOL) return false;

  rec.t = t1;
  rec.u = atan2(rec.p.z, rec.p.x) / (2 * PI) + 0.5;
  rec.v = 0.5 - asin(rec.p.y) / PI;
  rec.n = normalMapped(Vec(it.x, it.y, 0), rec);
  r.tMax = min(r.tMax, rec.t);
  return true;
}

bool Cylinder::hit(Ray& r, HitRec& rec) {
  Ray transformed = rayTransform(r);

  bool hit = false;
  if (hitBase( 1, transformed, rec)) hit = true;
  if (hitBase(-1, transformed, rec)) hit = true;
  if (hitSide(transformed, rec)) hit = true;
  if (hit) {
    rec.p = r.at(rec.t);
    rec.n = normalTransform(rec.n);
    r.tMax = min(r.tMax, rec.t);
    rec.obj = this;
  }
  return hit;
}

Vec Cylinder::sample(double *pdf, RNG& rng) {
  return T * Vec(0,0,0);
}