
/**
 * A surface of revolution turns itself into a mesh.
 */

#ifndef __PARAMETRIC_SURFACE_H__
#define __PARAMETRIC_SURFACE_H__

#include "objects/triangle_mesh.h"

struct ParametricSurface : TriangleMesh {
  ParametricSurface(BSDF *mat,
                    int aCount,  // Cuts along parameter a
                    int bCount,  // Cuts along parameter b
                    double aMin, // a min value
                    double aMax, // a max value
                    double bMin, // b min value
                    double bMax  // b max value
                    )
      : TriangleMesh(mat), aCount(aCount), aMin(aMin), aMax(aMax),
        bCount(bCount), bMin(bMin), bMax(bMax) {}

  void createSurface();

  void finalize() {
    createSurface();
    Object::finalize();
  }

  int aCount, bCount;
  double aMin, aMax;
  double bMin, bMax;

  // Parametric function to give points along surface
  virtual Vec P(double a, double b) = 0;

private:
  // Numerically compute normal
  Vec N(double a, double b);
  // Get Tex coords for point
  Vec T(double a, double b);
};

struct SphereParametric : ParametricSurface {
  SphereParametric(BSDF *mat,
                   int aCount = 20,   // Cuts along parameter a
                   int bCount = 20,   // Cuts along parameter b
                   double aMin = 0,   // a min value
                   double aMax = TAU, // a max value
                   double bMin = 0,   // b min value
                   double bMax = PI   // b max value
                   )
      : ParametricSurface(mat, aCount, bCount, aMin, aMax, bMin, bMax){};

  Vec P(double a, double b) {
    return Vec(cos(a) * sin(b), sin(a) * sin(b), cos(b));
  }
};

struct SaddleParametric : ParametricSurface {
  SaddleParametric(BSDF *mat,
                   int aCount = 20,  // Cuts along parameter a
                   int bCount = 20,  // Cuts along parameter b
                   double aMin = -1, // a min value
                   double aMax = 1,  // a max value
                   double bMin = -1, // b min value
                   double bMax = 1   // b max value
                   )
      : ParametricSurface(mat, aCount, bCount, aMin, aMax, bMin, bMax) {
    bothSides = true;
  };

  Vec P(double a, double b) {
    return Vec(a, b, a*a - b*b);
  }
};

#endif // __PARAMETRIC_SURFACE_H__