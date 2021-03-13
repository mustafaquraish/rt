#include "objects/cube.h"

bool Cube::hit(Ray& r, HitRec& rec) {
  Ray transformed = rayTransform(r);
  double t1, t2;

  AABB cube = AABB(1);

  if (!cube.hit(transformed, t1, t2)) return false;

  if (t1 > r.tMax) return false;
  if (t1 < TOL) t1 = t2;

  rec.t = t1;
  rec.p = r.at(rec.t);

  Vec it = transformed.at(rec.t);
  Vec canon_n;
  if (fabs(it.x - 1) < TOL)
    canon_n = Vec(+1, 0, 0), rec.u = it.y, rec.v = it.z;
  else if (fabs(it.x + 1) < TOL)
    canon_n = Vec(-1, 0, 0), rec.u = it.y, rec.v = it.z;
  else if (fabs(it.y - 1) < TOL)
    canon_n = Vec(0, +1, 0), rec.u = it.x, rec.v = it.z;
  else if (fabs(it.y + 1) < TOL)
    canon_n = Vec(0, -1, 0), rec.u = it.x, rec.v = it.z;
  else if (fabs(it.z - 1) < TOL)
    canon_n = Vec(0, 0, +1), rec.u = it.x, rec.v = it.y;
  else // (fabs(rec.p.z + 1) < TOL) 
    canon_n = Vec(0, 0, -1), rec.u = it.x, rec.v = it.y;

  rec.u = (rec.u + 1) / 2;
  rec.v = (rec.v + 1) / 2;
  
  canon_n = normalMapped(canon_n, rec);
  rec.n = normalTransform(canon_n);
  rec.obj = this;
  r.tMax = min(r.tMax, t1);
  return true;    

}

Vec Cube::sample(double *pdf, RNG& rng) {
  *pdf = 1 / surfaceArea;
  Vec p = Vec(rng.rand01(), rng.rand01(), 0.5) * 2 - 1;
  return T * p;
}