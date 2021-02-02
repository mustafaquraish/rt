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

  Vec canon_p = transformed.at(rec.t);
  if (fabs(canon_p.x - 1) < TOL)
    rec.n = normalTransform(Vec(+1, 0, 0));
  else if (fabs(canon_p.x + 1) < TOL)
    rec.n = normalTransform(Vec(-1, 0, 0));
  else if (fabs(canon_p.y - 1) < TOL)
    rec.n = normalTransform(Vec(0, +1, 0));
  else if (fabs(canon_p.y + 1) < TOL)
    rec.n = normalTransform(Vec(0, -1, 0));
  else if (fabs(canon_p.z - 1) < TOL)
    rec.n = normalTransform(Vec(0, 0, +1));
  else // (fabs(rec.p.z + 1) < TOL) 
    rec.n = normalTransform(Vec(0, 0, -1));

  rec.obj = this;
  rec.u = 0;
  rec.v = 0;
  r.tMax = min(r.tMax, t1);
  return true;    

}

Vec Cube::sample() {
  Vec p = Vec(rand01(), rand01(), 0.5) * 2 - 1;
  return T * p;
}