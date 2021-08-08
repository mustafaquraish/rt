#include <objects/plane.h>

bool Plane::hit(Ray& r, HitRec& rec) {
  Ray transformed = rayTransform(r);
  float t = -transformed.p.z / transformed.d.z;
  Vec3 it = transformed.at(t);
  if (t < TOL || t > r.tMax || 
      fabs(it.x) > 1 || fabs(it.y) > 1) {
    return false;
  } else {
    rec.t = t;
    rec.p = r.at(t);
    rec.uv = (Vec2(it.x, it.y) + 1) / 2.0;
    Vec3 canon_n = Vec3(0, 0, 1);
    canon_n = normalMapped(canon_n, rec);
    rec.n = normalTransform(canon_n);
    if (dot(r.d, rec.n) > 0) rec.n = -rec.n;
    rec.obj = this;
    r.tMax = min(r.tMax, t);
    return true;
  }
}

Vec3 Plane::sample(float *pdf, RNG& rng) {
  float x = rng.rand01() * 2 - 1;
  float y = rng.rand01() * 2 - 1;
  *pdf = 1 / surfaceArea;
  return T * Vec3(x, y, 0);
}

