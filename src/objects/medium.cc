#include <objects/medium.h>
#include <core/rng.h>

#define VOLUME_STEP_SIZE 0.1

bool Medium::hit(Ray& r, HitRec& rec) {
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

  for (float t = t1; t < t2; t += VOLUME_STEP_SIZE) {
    Vec3 pt = transformed.at(t);
    float density = m_density->get(pt);

    if (Random.rand01() < density) {
      rec.t = t;
      rec.p = r.at(t);
      rec.n = normalized(Random.randomVector());
      r.tMax = t;
      rec.uv = Vec2(0);
      if (this->bsdf) {
        rec.obj = this;
      } else {
        rec.obj = r1.obj;
      }
      return true;
    }
  }

  return false;
}

#undef VOLUME_STEP_SIZE