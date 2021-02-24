#ifndef __PERLIN_H__
#define __PERLIN_H__

#include "core/math.h"
#include "core/rng.h"

class Perlin {
public:
  Perlin(int count=16) : count(count) {
    RNG rng;
    ranvec = new Vec[count * count];
    for (int i = 0; i < count * count; ++i) {
      ranvec[i] = rng.randomVector();
    }

    perm_x = perlin_generate_perm(rng);
    perm_y = perlin_generate_perm(rng);
    perm_z = perlin_generate_perm(rng);
  }

  ~Perlin() {
    delete[] ranvec;
    delete[] perm_x;
    delete[] perm_y;
    delete[] perm_z;
  }

  double noise(const Vec &p) const {
    // Vec scaled = p;
    Vec scaled = p * count;
    int i = floor(scaled.x);
    int j = floor(scaled.y);
    int k = floor(scaled.z);
    double u = scaled.x - i;
    double v = scaled.y - j;
    double w = scaled.z - k;
    Vec c[2][2][2];

    for (int di = 0; di < 2; di++)
      for (int dj = 0; dj < 2; dj++)
        for (int dk = 0; dk < 2; dk++) {
          printf("%d %d %d\n", (i + di) & 255, (j + dj) & 255, (k + dk) & 255);
          c[di][dj][dk] =
              ranvec[perm_x[(i + di) & 255] ^ perm_y[(j + dj) & 255] ^
                     perm_z[(k + dk) & 255]];
        }

    return perlin_interp(c, u, v, w);
  }

  double turb(const Vec &p, int depth = 7) const {
    auto accum = 0.0;
    auto temp_p = p;
    auto weight = 1.0;

    for (int i = 0; i < depth; i++) {
      accum += weight * noise(temp_p);
      weight *= 0.5;
      temp_p *= 2;
    }

    return fabs(accum);
  }

private:
  int count = 16;
  Vec *ranvec;
  int *perm_x;
  int *perm_y;
  int *perm_z;

  int *perlin_generate_perm(RNG &rng) {
    auto p = new int[count * count];

    for (int i = 0; i < count * count; i++)
      p[i] = i;

    permute(p, count * count, rng);

    return p;
  }

  static void permute(int *p, int n, RNG &rng) {
    for (int i = n - 1; i > 0; i--) {
      int target = rng.randint(0, i);
      int tmp = p[i];
      p[i] = p[target];
      p[target] = tmp;
    }
  }

  static double perlin_interp(Vec c[2][2][2], double u, double v, double w) {
    auto uu = u * u * (3 - 2 * u);
    auto vv = v * v * (3 - 2 * v);
    auto ww = w * w * (3 - 2 * w);
    auto accum = 0.0;

    for (int i = 0; i < 2; i++)
      for (int j = 0; j < 2; j++)
        for (int k = 0; k < 2; k++) {
          Vec weight_v(u - i, v - j, w - k);
          accum += (i * uu + (1 - i) * (1 - uu)) *
                   (j * vv + (1 - j) * (1 - vv)) *
                   (k * ww + (1 - k) * (1 - ww)) * dot(c[i][j][k], weight_v);
        }
    return accum;
  }
};

#endif // __PERLIN_H__
