#ifndef __TERRAIN_H__
#define __TERRAIN_H__

// #include "core/perlin.h"
#include "ext/PerlinNoise.h"
#include "objects/parametric_surface.h"

struct PerlinTerrain : ParametricSurface {
  PerlinTerrain(BSDF *mat,
                int count = 20, 
                int detail = 4,
                double scale = 5
                )
      : ParametricSurface(mat, count, count, -1, 1, -1, 1), scale(scale) {
    // bothSides = true;
  };

  Vec P(double a, double b) {
    double height = perlin.noise2D_0_1((a+1)*4, (b+1)*4);
    // double height = perlin->turb(Vec(a, b, 1)) * scale;
    return Vec(a, b, height);
  }

  siv::BasicPerlinNoise<double> perlin;
  double scale;
};

#endif // __TERRAIN_H__