
/**
 * A surface of revolution turns itself into a mesh.
 */

#pragma once

#include <objects/triangle_mesh.h>
#include <vector>

struct ParametricSurface : TriangleMesh<Simple> {
  ParametricSurface(BSDF *mat,
                    int aCount,  // Cuts along parameter a
                    int bCount,  // Cuts along parameter b
                    float aMin, // a min value
                    float aMax, // a max value
                    float bMin, // b min value
                    float bMax  // b max value
                    )
      : TriangleMesh<Simple>(mat), aCount(aCount), aMin(aMin), aMax(aMax),
        bCount(bCount), bMin(bMin), bMax(bMax) {}

  std::vector<Primitive *> createSurface();

  void finalize();

  int aCount, bCount;
  float aMin, aMax;
  float bMin, bMax;

  bool interpolateNormals = true;

  // Parametric function to give points along surface
  virtual Vec3 P(float a, float b) = 0;

private:
  // Numerically compute normal
  Vec3 N(float a, float b);
  // Get Tex coords for point
  Vec2 T(float a, float b);
};

struct SphereParametric : ParametricSurface {
  SphereParametric(BSDF *mat,
                   int aCount = 20,   // Cuts along parameter a
                   int bCount = 20,   // Cuts along parameter b
                   float aMin = TOL,   // a min value
                   float aMax = TAU,   // a max value
                   float bMin = EPS,   // b min value
                   float bMax = PI-EPS // b max value
                   )
      : ParametricSurface(mat, aCount, bCount, aMin, aMax, bMin, bMax){};

  Vec3 P(float a, float b) {
    return Vec3(cos(a) * sin(b), sin(a) * sin(b), cos(b));
  }
};

struct SaddleParametric : ParametricSurface {
  SaddleParametric(BSDF *mat,
                   int aCount = 20,  // Cuts along parameter a
                   int bCount = 20,  // Cuts along parameter b
                   float aMin = -1, // a min value
                   float aMax = 1,  // a max value
                   float bMin = -1, // b min value
                   float bMax = 1   // b max value
                   )
      : ParametricSurface(mat, aCount, bCount, aMin, aMax, bMin, bMax) {
    bothSides = true;
  };

  Vec3 P(float a, float b) {
    return Vec3(a, b, a * a - b * b);
  }
};

