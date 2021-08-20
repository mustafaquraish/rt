#pragma once

#include <core/math.h>

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

  inline float rand01() {
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
    float u = rand01() * 2 - 1, theta = rand01() * TAU;
    float ra = sqrt(1 - u*u);
    return Vec3(
      ra * cos(theta),
      ra * sin(theta),
      u
    );
  }

  inline Vec3 randomUnitDisk() {
    float ang = rand01() * 2 * PI;
    float r = sqrt(rand01());
    return Vec3(cos(ang) * r, sin(ang) * r, 0);
  }

  inline Vec3 randomVectorCosineHemisphere(const Vec3 &n) {
    return normalized(n + randomVectorUnitSphere());
  }

  inline float randomNormalDist() {
    if (bm_has_cache) {
      bm_has_cache = false;
      return bm_cache;
    }
    float x1 = sqrt(-2 * log(rand01()));
    float f1 = rand01() * TAU;
    bm_cache = x1 * cos(f1);
    bm_has_cache = true;
    return x1 * sin(f1);
  }

  inline Vec3 randomVectorNormalDist(const Vec3 &mu, float sigma) {
    // https://en.wikipedia.org/wiki/Box%E2%80%93Muller_transform
    Vec3 dist = Vec3(randomNormalDist(),
                     randomNormalDist(),
                     randomNormalDist());
    return normalized(mu + dist * sigma);
  }
  
  int x;
  int y;
  int z;
  int w;

  bool bm_has_cache = false;
  float bm_cache;
};

inline thread_local RNG Random;