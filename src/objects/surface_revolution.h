#pragma once

/**
 * A surface of revolution tCount itself into a mesh.
 */

#include <objects/parametric_surface.h>

/**
 * A surface of revolution is just a parametric surface where parameter `a`
 *  is a custom function, and the parameter `b` just controls the rotation
 *  around the Y axis.
 */
struct SurfaceOfRevolution : ParametricSurface {
  SurfaceOfRevolution(BSDF *mat,
                      int tCount,  // Cuts along parameter a (function)
                      int rCount,  // Stops along revolution
                      double tMin, // t min value for defined F
                      double tMax  // t max value for defined F
                      )
      : ParametricSurface(mat, tCount, rCount, tMin, tMax, 0, TAU) {}

  Vec3 P(double a, double b);

  // Function to be rotated around Y axis
  virtual Vec3 F(double t) = 0;
};

struct CylinderSOR : SurfaceOfRevolution {
  CylinderSOR(BSDF *mat, 
              int tCount = 1, 
              int rCount = 20, 
              double tMin = 0,
              double tMax = 1
              )
      : SurfaceOfRevolution(mat, tCount, rCount, tMin, tMax){};

  Vec3 F(double t) {
    return Vec3(1, t, 0);
  }
};

struct SphereSOR : SurfaceOfRevolution {
  SphereSOR(BSDF *mat,
            int tCount = 5,
            int rCount = 20,
            double tMin = -PI / 2,
            double tMax = PI / 2)
      : SurfaceOfRevolution(mat, tCount, rCount, tMin, tMax){};

  Vec3 F(double t) {
    return Vec3(cos(t), sin(t), 0);
  }
};

struct TorusSOR : SurfaceOfRevolution {
  TorusSOR(BSDF *mat, 
           int tCount = 20, 
           int rCount = 20, 
           double tMin = 0,
           double tMax = 2 * PI)
      : SurfaceOfRevolution(mat, tCount, rCount, tMin, tMax){};

  Vec3 F(double t) {
    return Vec3(cos(t) + 3, sin(t), 0);
  }
};

struct SinSOR : SurfaceOfRevolution {
  SinSOR(BSDF *mat, 
         int tCount = 20, 
         int rCount = 20, 
         double tMin = 0,
         double tMax = 2 * PI)
      : SurfaceOfRevolution(mat, tCount, rCount, tMin, tMax){
        printf("SINSOR: %d %d\n", tCount, rCount);
      };

  Vec3 F(double t) {
    return Vec3(sin(t) + 2, t, 0);
  }
};

struct ParabolicBowlSOR : SurfaceOfRevolution {
  ParabolicBowlSOR(BSDF *mat, 
                   int tCount = 20, 
                   int rCount = 10,
                   double tMin = 0, 
                   double tMax = 2)
      : SurfaceOfRevolution(mat, tCount, rCount, tMin, tMax){};

  Vec3 F(double t) { return Vec3(t, t * t, 0); }
};

