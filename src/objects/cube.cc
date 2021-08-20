#include <objects/cube.h>
#include <core/rng.h>

bool Cube::hit(Ray& r, HitRec& rec) {
  Ray transformed = rayTransform(r);
  float t1, t2;

  AABB cube = AABB(1);

  if (!cube.hit(transformed, t1, t2)) return false;

  if (t1 > r.tMax) return false;
  if (t1 < TOL) t1 = t2;

  rec.t = t1;
  rec.p = r.at(rec.t);

  Vec3 it = transformed.at(rec.t);
  Vec3 canon_n;
  if (fabs(it.x - 1) < TOL)
    canon_n = Vec3(+1, 0, 0), rec.uv = Vec2(it.y, it.z);
  else if (fabs(it.x + 1) < TOL)
    canon_n = Vec3(-1, 0, 0), rec.uv = Vec2(it.y, it.z);
  else if (fabs(it.y - 1) < TOL)
    canon_n = Vec3(0, +1, 0), rec.uv = Vec2(it.x, it.z);
  else if (fabs(it.y + 1) < TOL)
    canon_n = Vec3(0, -1, 0), rec.uv = Vec2(it.x, it.z);
  else if (fabs(it.z - 1) < TOL)
    canon_n = Vec3(0, 0, +1), rec.uv = Vec2(it.x, it.y);
  else // (fabs(rec.p.z + 1) < TOL) 
    canon_n = Vec3(0, 0, -1), rec.uv = Vec2(it.x, it.y);

  rec.uv = (rec.uv + 1) / 2;
  
  canon_n = normalMapped(canon_n, rec);
  rec.n = normalTransform(canon_n);
  rec.obj = this;
  r.tMax = min(r.tMax, t1);
  return true;    

}

Vec3 Cube::sample(float *pdf, RNG& rng) {
  *pdf = 1 / surfaceArea;
  Vec3 p = Vec3(rng.rand01(), rng.rand01(), 0.5) * 2 - 1;
  return T * p;
}