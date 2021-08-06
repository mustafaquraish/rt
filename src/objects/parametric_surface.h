
/**
 * A surface of revolution turns itself into a mesh.
 */

#pragma once

#include <objects/triangle_mesh.h>
#include <vector>

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

  std::vector<Primitive *> createSurface();

  void finalize();

  int aCount, bCount;
  double aMin, aMax;
  double bMin, bMax;

  bool interpolateNormals = true;

  // Parametric function to give points along surface
  virtual Vec3 P(double a, double b) = 0;

private:
  // Numerically compute normal
  Vec3 N(double a, double b);
  // Get Tex coords for point
  Vec3 T(double a, double b);
};

struct SphereParametric : ParametricSurface {
  SphereParametric(BSDF *mat,
                   int aCount = 20,   // Cuts along parameter a
                   int bCount = 20,   // Cuts along parameter b
                   double aMin = TOL,   // a min value
                   double aMax = TAU,   // a max value
                   double bMin = EPS,   // b min value
                   double bMax = PI-EPS // b max value
                   )
      : ParametricSurface(mat, aCount, bCount, aMin, aMax, bMin, bMax){};

  Vec3 P(double a, double b) {
    return Vec3(cos(a) * sin(b), sin(a) * sin(b), cos(b));
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

  Vec3 P(double a, double b) {
    return Vec3(a, b, a * a - b * b);
  }
};

