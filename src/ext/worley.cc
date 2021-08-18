#include <ext/worley.h>
#include <core/rng.h>
#include <core/math.h>

namespace Worley {

#define INV_SQRT_2 0.7071067811865475
#define GRID_SIZE 10

static const struct WorleyCache {
  WorleyCache() {
    for (int i = 0; i < GRID_SIZE; i++)
      for (int j = 0; j < GRID_SIZE; j++)
        for (int k = 0; k < GRID_SIZE; k++) {
          data[i][j][k] = Vec3(
            Random.rand01(), 
            Random.rand01(), 
            Random.rand01()
          );
        }
  }
  Vec3 data[GRID_SIZE][GRID_SIZE][GRID_SIZE];
} s_cache;

int modulo(int x, int N){
  int t = x % N;
  return t < 0 ? N + t : t;
}

Vec3 sample(int x, int y, int z) {
  x = modulo(x, GRID_SIZE);
  y = modulo(y, GRID_SIZE);
  z = modulo(z, GRID_SIZE);
  return s_cache.data[x][y][z];
}

void check_index(const Vec3& p, int x, int y, int z, float &min_dist) {
  Vec3 pt = Vec3(x, y, z) + sample(x, y, z);
  float dist = length_sq(pt - p);
  if (dist < min_dist) {
    min_dist = dist;
  }
}

float noise(const Vec3& p) {
  int ix = p.x < 0 ? p.x - 1 : p.x;
  int iy = p.y < 0 ? p.y - 1 : p.y;
  int iz = p.z < 0 ? p.z - 1 : p.z;

  float min_dist = INFINITY;
  check_index(p, ix - 1, iy - 1, iz - 1, min_dist);
  check_index(p, ix - 1, iy - 1, iz + 0, min_dist);
  check_index(p, ix - 1, iy - 1, iz + 1, min_dist);
  check_index(p, ix - 1, iy + 0, iz - 1, min_dist);
  check_index(p, ix - 1, iy + 0, iz + 0, min_dist);
  check_index(p, ix - 1, iy + 0, iz + 1, min_dist);
  check_index(p, ix - 1, iy + 1, iz - 1, min_dist);
  check_index(p, ix - 1, iy + 1, iz + 0, min_dist);
  check_index(p, ix - 1, iy + 1, iz + 1, min_dist);
  check_index(p, ix + 0, iy - 1, iz - 1, min_dist);
  check_index(p, ix + 0, iy - 1, iz + 0, min_dist);
  check_index(p, ix + 0, iy - 1, iz + 1, min_dist);
  check_index(p, ix + 0, iy + 0, iz - 1, min_dist);
  check_index(p, ix + 0, iy + 0, iz + 0, min_dist);
  check_index(p, ix + 0, iy + 0, iz + 1, min_dist);
  check_index(p, ix + 0, iy + 1, iz - 1, min_dist);
  check_index(p, ix + 0, iy + 1, iz + 0, min_dist);
  check_index(p, ix + 0, iy + 1, iz + 1, min_dist);
  check_index(p, ix + 1, iy - 1, iz - 1, min_dist);
  check_index(p, ix + 1, iy - 1, iz + 0, min_dist);
  check_index(p, ix + 1, iy - 1, iz + 1, min_dist);
  check_index(p, ix + 1, iy + 0, iz - 1, min_dist);
  check_index(p, ix + 1, iy + 0, iz + 0, min_dist);
  check_index(p, ix + 1, iy + 0, iz + 1, min_dist);
  check_index(p, ix + 1, iy + 1, iz - 1, min_dist);
  check_index(p, ix + 1, iy + 1, iz + 0, min_dist);
  check_index(p, ix + 1, iy + 1, iz + 1, min_dist);
  return min_dist * INV_SQRT_2;
}

float noise(float x, float y) { 
  const Vec3 p(x, y, 0);
  int ix = x < 0 ? x - 1 : x;
  int iy = y < 0 ? y - 1 : y;
  float min_dist = INFINITY;
  check_index(p, ix - 1, iy - 1, 0, min_dist);
  check_index(p, ix - 1, iy + 0, 0, min_dist);
  check_index(p, ix - 1, iy + 1, 0, min_dist);
  check_index(p, ix + 0, iy - 1, 0, min_dist);
  check_index(p, ix + 0, iy + 0, 0, min_dist);
  check_index(p, ix + 0, iy + 1, 0, min_dist);
  check_index(p, ix + 1, iy - 1, 0, min_dist);
  check_index(p, ix + 1, iy + 0, 0, min_dist);
  check_index(p, ix + 1, iy + 1, 0, min_dist);
  return min_dist * INV_SQRT_2;
}

float noise(const Vec2& p) { return noise(p.x, p.y); }
float noise(float x, float y, float z) { return noise(Vec3(x, y, z)); }

float layered(const Vec2& p, int octaves, float persistence, float lacunarity) {
  float total = 0, freq = 1, amplitude = 1, maxAmplitude = 0;
  
  for (int i = 0; i < octaves; i++) {
    total += noise(p * freq) * amplitude;
    freq *= lacunarity, maxAmplitude += amplitude, amplitude *= persistence;
  }

  return total / maxAmplitude;
}

float layered(const Vec3& p, int octaves, float persistence, float lacunarity) {
  float total = 0, freq = 1, amplitude = 1, maxAmplitude = 0;
  
  for (int i = 0; i < octaves; i++) {
    total += noise(p * freq) * amplitude;
    freq *= lacunarity, maxAmplitude += amplitude, amplitude *= persistence;
  }

  return total / maxAmplitude;
}

float layered(float x, float y, int octaves, float persistence, float lacunarity) {
  return layered(Vec2(x, y), octaves, persistence, lacunarity);
}

float layered(float x, float y, float z, int octaves, float persistence, float lacunarity) {
  return layered(Vec3(x, y, z), octaves, persistence, lacunarity);
}



#undef INV_SQRT_2

}