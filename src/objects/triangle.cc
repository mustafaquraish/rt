/**
 * This is NOT an object, but it just extends Primitive. Used in the 
 * TriangleMesh() object.
 */

#include "objects/triangle.h"

Triangle::Triangle(Vec p0, Vec p1, Vec p2,
                   Vec n0, Vec n1, Vec n2,
                   Vec t0, Vec t1, Vec t2) {

  p[0] = p0; n[0] = n0; uv[0] = t0;
  p[1] = p1; n[1] = n1; uv[1] = t1;
  p[2] = p2; n[2] = n2; uv[2] = t2;

  bounds = AABB(p0);
  bounds.addPoint(p1);
  bounds.addPoint(p2);
};

Triangle::Triangle(Vec  p0, Vec  p1, Vec  p2,
                   Vec  n0, Vec  n1, Vec  n2) {

  p[0] = p0; n[0] = n0;
  p[1] = p1; n[1] = n1;
  p[2] = p2; n[2] = n2;

  bounds = AABB(p0);
  bounds.addPoint(p1);
  bounds.addPoint(p2);
};

Triangle::Triangle(Vec  p0, Vec  p1, Vec  p2) {

  Vec nn = cross(p1-p0, p2-p0);
  p[0] = p0; n[0] = nn;
  p[1] = p1; n[1] = nn;
  p[2] = p2; n[2] = nn;

  bounds = AABB(p0);
  bounds.addPoint(p1);
  bounds.addPoint(p2);
};

bool Triangle::hit(Ray& r, HitRec& rec) {
  double v, w, u, t;
  
  Vec e0 = p[1] - p[0];
  Vec e1 = p[2] - p[0];

  Vec h = cross(r.d, e1);
  double a1 = dot(e0, h);
  if (a1 == 0) return false;

  double f = 1.0 / a1;

  Vec s = r.p - p[0];
  v = f * dot(s, h);
  if (v < 0.0 || v > 1.0) return false;

  Vec q = cross(s, e0);
  w = f * dot(r.d, q);
  if (w < 0.0 || v + w > 1.0) return false;
  
  t = f * dot(e1, q);
  if (t < TOL || t > r.tMax) return false;

  u = 1 - v - w;

  // NOTE: rec.p needs to be set by `TriangleMesh::hit`
  // NOTE: rec.n needs to transformed by `TriangleMesh::hit`
  rec.t = t;
  rec.n = Vec(n[0].x * u + n[1].x * v + n[2].x * w,
              n[0].y * u + n[1].y * v + n[2].y * w,
              n[0].z * u + n[1].z * v + n[2].z * w);
  rec.u = uv[0].x * u + uv[1].x * v + uv[2].x * w;
  rec.v = uv[0].y * u + uv[1].y * v + uv[2].y * w;
  r.tMax = min(r.tMax, t);

  return 1;
}