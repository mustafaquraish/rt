/**
 * Displacement-mapped objects
 */

#pragma once

#include <objects/parametric_surface.h>

struct DisplacedSphere : ParametricSurface {
  DisplacedSphere(
    Texture *displacementMap, 
    BSDF *mat, 
    double scale=.2, 
    int resolution = 100, 
    bool interpNormals=true
  ) : ParametricSurface(mat, resolution, resolution, 0, TAU, EPS, PI-EPS),
      displacementMap(displacementMap), 
      scale(scale) {
    interpolateNormals = interpNormals;
  };

  Vec3 P(double a, double b) {
    double u = map(a, aMin, aMax, 0, 1);
    double v = map(b, bMin, bMax, 0, 1);
    double offset = luminance(displacementMap->get(u, v)) * scale;
    
    Vec3 pt = Vec3(cos(a) * sin(b), sin(a) * sin(b), cos(b));
    return pt * (1+offset);
  }
  double scale;
  Texture *displacementMap;
};

struct DisplacedPlane : ParametricSurface {
  DisplacedPlane(
    Texture *displacementMap, 
    BSDF *mat, 
    double scale=.2,
    int resolution = 100, 
    bool interpNormals=true
  ) : ParametricSurface(mat, resolution, resolution, -1, 1, -1, 1),
      displacementMap(displacementMap), 
      scale(scale) {
    interpolateNormals = interpNormals;
  };

  Vec3 P(double a, double b) {
    double u = map(a, aMin, aMax, 0, 1);
    double v = map(b, bMin, bMax, 0, 1);
    double offset = luminance(displacementMap->get(u, v)) * scale;
    return Vec3(a, b, offset);
  }
  double scale;
  Texture *displacementMap;
};
