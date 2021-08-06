#include <objects/disc.h>


bool Disc::hit(Ray& r, HitRec& rec) {
  Ray transformed = rayTransform(r);

  float t = -transformed.p.z / transformed.d.z;
  if (t < TOL || t > r.tMax) return false;

  Vec3 it = transformed.at(t);
  float xy_dist = it.x * it.x + it.y * it.y;
  if (fabs(xy_dist) > 1+TOL) return false;

  rec.p = r.at(t);
  rec.t = t;
  rec.u = (it.x + 1) / 2;
  rec.v = (it.y + 1) / 2;
  rec.obj = this;

  Vec3 canon_n = normalMapped(Vec3(0, 0, 1), rec);
  rec.n = normalTransform(canon_n);
  r.tMax = min(r.tMax, rec.t);

  return true;
}

Vec3 Disc::sample(float *pdf, RNG& rng) {
  float x, y;
  do {
    x = 2 * rng.rand01() - 1;
    y = 2 * rng.rand01() - 1;
  } while (x*x + y*y > 1);

  *pdf = 1 / surfaceArea;
  return T * Vec3(x, y, 0);
}