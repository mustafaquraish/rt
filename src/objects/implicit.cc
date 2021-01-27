#include "objects/implicit.h"

double RAYMARCH_MAX_LAMBDA = 100;
double RAYMARCH_INCREMENT = 0.005;

bool Implicit::hit(Ray& r, HitRec& rec) {
  Ray transformed = rayTransform(r);

  double tmin, tmax;
  if (!localBouds.hit(transformed, tmin, tmax)) return false;

  // printf("IMPLICIT BOUNDS ARE %f %f\n", tmin, tmax);

  double inc = RAYMARCH_INCREMENT;

  double lambda = max(10e-3, tmin);
  double maxLambda = min(r.tMax, tmax + inc);

  Vec p = transformed.at(lambda);

  double v0 = F(p.x, p.y, p.z);

  while (lambda < maxLambda) {
    p = transformed.at(lambda);
    double v1 = F(p.x, p.y, p.z);

    // Intersection!
    if (v1 < TOL && v1 > -TOL) {
      v0 = v1;
      break;

    // Crossed a boundary. Reduce increment by half and binary search..
    } else if (v0 * v1 < 0) {
      inc *= 0.5;
      lambda -= inc;

    // Otherwise, still haven't reached an intersectio. Move on.
    } else {
      v0 = v1;
      lambda += inc;
    }
  }

  if (lambda >= maxLambda) {
    return false;
  }

  double EPS = 0.0001;
  Vec canon_n = (Vec(F(p.x + EPS, p.y , p.z),
                     F(p.x , p.y + EPS, p.z),
                     F(p.x , p.y , p.z + EPS)) - v0) / EPS;

  rec.t = lambda;
  rec.p = r.at(lambda);
  rec.n = normalTransform(canon_n);
  rec.a = rec.b = 0;
  rec.obj = this;
  r.tMax = min(r.tMax, lambda);
  return true;
}

void Implicit::finalize() {
  T_inv = invert(T);
  localBouds = bounds;
  bounds = transformed(T, bounds);
}


Vec Implicit::sample() {
  return T * Vec();
}

