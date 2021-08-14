#include <objects/constant_medium.h>
#include <core/rng.h>

bool ConstantMedium::hit(Ray& r, HitRec& rec) {
  Ray transformed = rayTransform(r);

  HitRec r1, r2;
  float t1 = 0, t2 = 0;
  
  if (!m_obj->hit(transformed, r1)) { return false; }

  if (dot(r1.n, transformed.d) < 0) {
    t1 = r1.t;
    Ray temp_ray = Ray(transformed.at(t1), transformed.d, r.tMax);
    if (m_obj->hit(temp_ray, r2)) {
      t2 = t1 + r2.t;
    } else {
      t2 = r.tMax;
    }
  } else {
    t2 = r1.t;
  }

  double dst = (t2 - t1);
  double hd = -(1/m_density) * log(Random.rand01());

  if (hd < dst) {
    rec.t = t1 + hd;
    rec.p = r.at(rec.t);
    rec.uv = Vec2(0, 0);
    rec.n = normalized(Random.randomVector());
    if (this->bsdf) {
      rec.obj = this; 
    } else {
      rec.obj = r1.obj;
    }
    r.tMax = rec.t;
    return true;
  }

  return false;
}