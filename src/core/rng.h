#pragma once

#include <core/math.h>
#include <time.h>

struct RNG {
  RNG() {
    x = rand();
    y = rand();
    z = rand();
    w = rand();
  }

  RNG(int seed) {
    srand(seed);
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

  inline int randint() {
    int t = x ^ (x << 11);
    x = y, y = z, z = w;
    return (w = w ^ (w >> 19) ^ (t ^ (t >> 8)));
  }

  inline int randint(int min, int max) {
    int t = x ^ (x << 11);
    x = y, y = z, z = w;
    int r = (w = w ^ (w >> 19) ^ (t ^ (t >> 8)));
    return r % (max - min + 1) + min;
  }

  inline Vec3 randomVector() {
    return 2 * Vec3(rand01(), rand01(), rand01()) - 1;
  }

  inline Vec3 randomVectorUnitSphere() {
    while (true) {
      Vec3 p = randomVector();
      if (lengthSq(p) >= 1) continue;
      return p;
    }
  }

  inline Vec3 randomUnitDisk() {
    double ang = rand01() * 2 * PI;
    double r = sqrt(rand01());
    return Vec3(cos(ang) * r, sin(ang) * r, 0);
  }

  inline Vec3 randomVectorCosineHemisphere(const Vec3 &n) {
    return norm(n + norm(randomVectorUnitSphere()));
  }
  
  int x;
  int y;
  int z;
  int w;
};

