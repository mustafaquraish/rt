#ifndef __SURFACE_REVOLUTION_H__
#define __SURFACE_REVOLUTION_H__

/**
 * A surface of revolution turns itself into a mesh.
 */

#include "objects/triangle_mesh.h"

struct SurfaceOfRevolution : TriangleMesh {
  SurfaceOfRevolution(
    Material *mat, int turns, int loops, double tMin, double tMax
  ) : TriangleMesh(mat), turns(turns), loops(loops), tMin(tMin), tMax(tMax) {}

  void createSurface();

  // Function to rotate around Y axis
  virtual Vec F(double t) = 0;
  void finalize() {
    createSurface();
    Object::finalize();
  }

  int turns;
  int loops;
  double tMin;
  double tMax;

private:
  // Numerically compute normal
  Vec N(double t);
};

struct CylinderSOR : SurfaceOfRevolution {
  CylinderSOR(
    Material *mat, int turns=20, int loops=1, double tMin=0, double tMax=1
  ) : SurfaceOfRevolution(mat, turns, loops, tMin, tMax) {};

  Vec F(double t) {
    return Vec(1, t, 0);
  }
};

struct SphereSOR : SurfaceOfRevolution {
  SphereSOR(
    Material *mat, int turns=20, int loops=10, double tMin=-PI/2, double tMax=PI/2
  ) : SurfaceOfRevolution(mat, turns, loops, tMin, tMax) {};

  Vec F(double t) {
    return Vec(cos(t), sin(t), 0);
  }
};

struct TorusSOR : SurfaceOfRevolution {
  TorusSOR(
    Material *mat, int turns=20, int loops=20, double tMin=0, double tMax=2*PI
  ) : SurfaceOfRevolution(mat, turns, loops, tMin, tMax) {};

  Vec F(double t) {
    return Vec(cos(t)+3, sin(t), 0);
  }
};

struct SinSOR : SurfaceOfRevolution {
  SinSOR(
    Material *mat, int turns=20, int loops=10, double tMin=0, double tMax=2*PI
  ) : SurfaceOfRevolution(mat, turns, loops, tMin, tMax) {};

  Vec F(double t) {
    return Vec(sin(t) + 2, t, 0);
  }
};

struct ParabolicBowlSOR : SurfaceOfRevolution {
  ParabolicBowlSOR(
    Material *mat, int turns=20, int loops=10, double tMin=0, double tMax=2
  ) : SurfaceOfRevolution(mat, turns, loops, tMin, tMax) {};

  Vec F(double t) {
    return Vec(t, t*t, 0);
  }
};

#endif // __SURFACE_REVOLUTION_H__
