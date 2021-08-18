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
  
  int x;
  int y;
  int z;
  int w;
};

inline thread_local RNG Random;