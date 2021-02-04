#ifndef __RNG_H__
#define __RNG_H__

#include "core/math.h"

struct RNG {
  RNG() {
    x = rand();
    y = rand();
    z = rand();
    w = rand();
  }

  inline double rand01() {
    int t = x ^ (x << 11);
    x = y, y = z, z = w;
    return (w = w ^ (w >> 19) ^ (t ^ (t >> 8))) * (1/2147483647.0);
  }

  inline Vec randomVector() {
    return norm(2 * Vec(rand01(), rand01(), rand01()) - 1);
  }

  inline Vec randomVectorUnitSphere() {
    while (true) {
      Vec p = randomVector();
      if (lengthSq(p) >= 1) continue;
      return p;
    }
  }

  inline Vec randomVectorCosineHemisphere(const Vec &n) {
    // Random sample on hemisphere with cosine-weighted distribution
    double r = sqrt(rand01());
    double theta = 2 * PI * rand01();

    double x = r * cos(theta);
    double y = r * sin(theta);
    double z = sqrt(1.0 - (x * x) - (y * y));
    Vec d = Vec(x, y, z);

    // Transform from hemisphere to normal-space
    return getRotationMatrix(n) * d;
  }
  
  int x;
  int y;
  int z;
  int w;
};

#endif // __RNG_H__
