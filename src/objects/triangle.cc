#include "objects/triangle.h"

Vec Triangle::baryCentricWeights(const Vec& pt) {
  Vec AB = p[1] - p[0], AC = p[2] - p[0];
  Vec BP =   pt - p[1], CP =   pt - p[2];
  double arABC = length(cross(AB, AC));
  double u = length(cross(BP, CP)) / arABC;
  double v = length(cross(AC, CP)) / arABC;
  double w = 1 - u - v;
  return Vec(u, v, w);
}

bool Triangle::hit(Ray& ray, HitRec& rec) {
  // printf("Hit.. triangle...\n");

  double u, v, w, t;
  
  Vec e0 = p[1] - p[0];
  Vec e1 = p[2] - p[0];

  Vec h = cross(ray.d, e1);
  double a1 = dot(e0, h);
  if (a1 < TOL && a1 > -TOL) {
    return 0;
  }

  double f = 1.0 / a1;

  Vec s = ray.p - p[0];
  u = f * dot(s, h);
  if (u < 0.0 || u > 1.0) {
    return 0;;
  }

  Vec q = cross(s, e0);
  v = f * dot(ray.d, q);
  if (v < 0.0 || u + v > 1.0) {
    return 0;
  }
  t = f * dot(e1, q);

  if (t < TOL) return 0;


  Vec bw = baryCentricWeights(ray.at(t));

  /*** rec.p   NEEDS TO BE SET BY THE CALLER ***/
  rec.t1 = t;
  // Needs to be transformed by TriangleMesh class.
  rec.n = Vec(n[0].x * bw[0] + n[1].x * bw[1] + n[2].x * bw[2],
              n[0].y * bw[0] + n[1].y * bw[1] + n[2].y * bw[2],
              n[0].z * bw[0] + n[1].z * bw[1] + n[2].z * bw[2]);
  rec.a = ab[0].x * bw[0] + ab[1].x * v + ab[2].x * w;
  rec.b = ab[0].y * bw[0] + ab[1].y * v + ab[2].y * w;
  
  return 1;
}

// AABB Triangle::getBounds() const {
//   AABB bounds = AABB(0);
//   bounds.addPoint(p[0]);
//   bounds.addPoint(p[1]);
//   bounds.addPoint(p[2]);
//   return bounds;
// }