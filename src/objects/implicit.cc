#include <objects/implicit.h>

#define RAYMARCH_MAX_LAMBDA 100
#define RAYMARCH_INCREMENT 0.005

bool Implicit::hit(Ray& r, HitRec& rec) {
  Ray transformed = rayTransform(r);

  float tmin, tmax;
  if (!localBouds.hit(transformed, tmin, tmax)) return false;

  float inc = RAYMARCH_INCREMENT;

  float lambda = max(10e-3, tmin);
  float maxLambda = min(r.tMax, tmax + inc);

  Vec3 p = transformed.at(lambda);

  float v0 = F(p.x, p.y, p.z);

  while (lambda < maxLambda) {
    p = transformed.at(lambda);
    float v1 = F(p.x, p.y, p.z);

    // Intersection!
    if (v1 < TOL && v1 > -TOL) {
      v0 = v1;
      break;

    // Crossed a boundary. Reduce increment by half and binary search..
    } else if (v0 * v1 < 0) {
      inc *= 0.5;
      lambda -= inc;

    // Otherwise, still haven't reached an intersection. Move on.
    } else {
      v0 = v1;
      lambda += inc;
    }
  }

  if (lambda >= maxLambda) {
    return false;
  }

  Vec3 canon_n = (Vec3(F(p.x + EPS, p.y , p.z),
                       F(p.x , p.y + EPS, p.z),
                       F(p.x , p.y , p.z + EPS)) - v0) / EPS;
  canon_n = normalized(canon_n);

  rec.t = lambda;
  rec.p = r.at(lambda);

  rec.uv = Vec2(atan2(canon_n.z, canon_n.x) / (2 * PI) + 0.5,
                0.5 - asin(canon_n.y) / PI);

  canon_n = normalMapped(canon_n, rec);
  rec.n = normalTransform(canon_n);

  rec.obj = this;
  r.tMax = min(r.tMax, lambda);
  return true;
}

void Implicit::finalize() {
  T_inv = invert(T);
  localBouds = bounds;
  bounds = transformed(T, bounds);
}


Vec3 Implicit::sample(float *pdf, RNG& rng) {
  return T * Vec3();
}

