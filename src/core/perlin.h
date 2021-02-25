/**
 * Based off Ken Perlin's Java Reference Implementation:
 *    https://mrl.cs.nyu.edu/~perlin/noise/
 *
 */

#ifndef __PERLIN_H__
#define __PERLIN_H__

#include "core/math.h"

struct Perlin {

  static double noise(const Vec& pt) {
    return noise(pt.x, pt.y, pt.z);
  }

  static double noise(double x, double y, double z) {
    // Find unit cube that contains point
    int X = ((int) x) & 255;
    int Y = ((int) y) & 255;
    int Z = ((int) z) & 255;

    // Find relative offset within unit cube
    x = x - (int)x;
    y = y - (int)y;
    z = z - (int)z;

    // Compute fade curves for components
    double u = fade(x);
    double v = fade(y);
    double w = fade(z);

    // Hash coordinates of cube corners
    int A = p[X    ] + Y; int AA = p[A] + Z; int AB = p[A + 1] + Z;
    int B = p[X + 1] + Y; int BA = p[B] + Z; int BB = p[B + 1] + Z;

    // Trilinearly interpolate values
    return trilerp(
      u, v, w, 
      grad(p[AA    ], x    , y    , z    ), 
      grad(p[BA    ], x - 1, y    , z    ),
      grad(p[AB    ], x    , y - 1, z    ), 
      grad(p[BB    ], x - 1, y - 1, z    ),
      grad(p[AA + 1], x    , y    , z - 1),
      grad(p[BA + 1], x - 1, y    , z - 1),
      grad(p[AB + 1], x    , y - 1, z - 1),
      grad(p[BB + 1], x - 1, y - 1, z - 1)
    );
  }

  static double layered(const Vec& pt, int octaves=4,
                        double persistance=0.5, double lacunarity=2) {

    double amplitude = 1;
    double perlinValue = 0;
    Vec scaled_pt = pt;
    
    for (int i = 0; i < octaves; i++) {
      perlinValue += noise(scaled_pt) * amplitude;

      amplitude *= persistance;
      scaled_pt *= lacunarity;
    }
    return perlinValue;
  }

  static double layered(double x, double y, double z, int octaves=4,
                        double persistance=0.5, double lacunarity=2) {
    return layered(Vec(x,y,z), octaves, persistance, lacunarity);
  }

  // Converts the low 4-bits of hash code into 12 gradient directions
  static double grad(int hash, double x, double y, double z) {
    int h = hash & 15;
    double u = (h < 8) ? x : y;
    double v = (h < 4) ? y : ((h == 12 || h == 14) ? x : z);
    return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
  }

 private:
  inline static int p[512] = {
    151, 160, 137, 91,  90,  15,  131, 13,  201, 95,  96,  53,  194, 233, 7,
    225, 140, 36,  103, 30,  69,  142, 8,   99,  37,  240, 21,  10,  23,  190,
    6,   148, 247, 120, 234, 75,  0,   26,  197, 62,  94,  252, 219, 203, 117,
    35,  11,  32,  57,  177, 33,  88,  237, 149, 56,  87,  174, 20,  125, 136,
    171, 168, 68,  175, 74,  165, 71,  134, 139, 48,  27,  166, 77,  146, 158,
    231, 83,  111, 229, 122, 60,  211, 133, 230, 220, 105, 92,  41,  55,  46,
    245, 40,  244, 102, 143, 54,  65,  25,  63,  161, 1,   216, 80,  73,  209,
    76,  132, 187, 208, 89,  18,  169, 200, 196, 135, 130, 116, 188, 159, 86,
    164, 100, 109, 198, 173, 186, 3,   64,  52,  217, 226, 250, 124, 123, 5,
    202, 38,  147, 118, 126, 255, 82,  85,  212, 207, 206, 59,  227, 47,  16,
    58,  17,  182, 189, 28,  42,  223, 183, 170, 213, 119, 248, 152, 2,   44,
    154, 163, 70,  221, 153, 101, 155, 167, 43,  172, 9,   129, 22,  39,  253,
    19,  98,  108, 110, 79,  113, 224, 232, 178, 185, 112, 104, 218, 246, 97,
    228, 251, 34,  242, 193, 238, 210, 144, 12,  191, 179, 162, 241, 81,  51,
    145, 235, 249, 14,  239, 107, 49,  192, 214, 31,  181, 199, 106, 157, 184,
    84,  204, 176, 115, 121, 50,  45,  127, 4,   150, 254, 138, 236, 205, 93,
    222, 114, 67,  29,  24,  72,  243, 141, 128, 195, 78,  66,  215, 61,  156,
    180, 151, 160, 137, 91,  90,  15,  131, 13,  201, 95,  96,  53,  194, 233,
    7,   225, 140, 36,  103, 30,  69,  142, 8,   99,  37,  240, 21,  10,  23,
    190, 6,   148, 247, 120, 234, 75,  0,   26,  197, 62,  94,  252, 219, 203,
    117, 35,  11,  32,  57,  177, 33,  88,  237, 149, 56,  87,  174, 20,  125,
    136, 171, 168, 68,  175, 74,  165, 71,  134, 139, 48,  27,  166, 77,  146,
    158, 231, 83,  111, 229, 122, 60,  211, 133, 230, 220, 105, 92,  41,  55,
    46,  245, 40,  244, 102, 143, 54,  65,  25,  63,  161, 1,   216, 80,  73,
    209, 76,  132, 187, 208, 89,  18,  169, 200, 196, 135, 130, 116, 188, 159,
    86,  164, 100, 109, 198, 173, 186, 3,   64,  52,  217, 226, 250, 124, 123,
    5,   202, 38,  147, 118, 126, 255, 82,  85,  212, 207, 206, 59,  227, 47,
    16,  58,  17,  182, 189, 28,  42,  223, 183, 170, 213, 119, 248, 152, 2,
    44,  154, 163, 70,  221, 153, 101, 155, 167, 43,  172, 9,   129, 22,  39,
    253, 19,  98,  108, 110, 79,  113, 224, 232, 178, 185, 112, 104, 218, 246,
    97,  228, 251, 34,  242, 193, 238, 210, 144, 12,  191, 179, 162, 241, 81,
    51,  145, 235, 249, 14,  239, 107, 49,  192, 214, 31,  181, 199, 106, 157,
    184, 84,  204, 176, 115, 121, 50,  45,  127, 4,   150, 254, 138, 236, 205,
    93,  222, 114, 67,  29,  24,  72,  243, 141, 128, 195, 78,  66,  215, 61,
    156, 180,
  };
};

#endif  // __PERLIN_H__
