/**
 * Displacement-mapped objects
 */

#ifndef __DISPLACED_H__
#define __DISPLACED_H__

#include "objects/parametric_surface.h"

struct DisplacedSphere : ParametricSurface {
  DisplacedSphere(
    Texture *displacementMap, 
    BSDF *mat, 
    double scale=.2, 
    int resolution = 100, 
    bool interpNormals=true
  ) : ParametricSurface(mat, resolution, resolution, 0, TAU, 0, PI),
      displacementMap(displacementMap), 
      scale(scale) {
    interpolateNormals = interpNormals;
  };

  Vec P(double a, double b) {
    double u = map(a, aMin, aMax, 0, 1);
    double v = map(b, bMin, bMax, 0, 1);
    double offset = luminance(displacementMap->get(u, v)) * scale;
    
    Vec pt = Vec(cos(a) * sin(b), sin(a) * sin(b), cos(b));
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

  Vec P(double a, double b) {
    double u = map(a, aMin, aMax, 0, 1);
    double v = map(b, bMin, bMax, 0, 1);
    double offset = luminance(displacementMap->get(u, v)) * scale;
    return Vec(a,b,offset);
  }
  double scale;
  Texture *displacementMap;
};


#endif // __Displaced_H__
