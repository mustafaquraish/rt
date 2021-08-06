
/**
 * A surface of revolution rCount itself into a mesh.
 */
#pragma once


#include <objects/parametric_surface.h>

/**
 * A Beveled Curve is just a parametric surface where parameter `a`
 *  is a custom function, and the parameter `b` is reponsible for a point
 *  on a circle around the curve
 */
struct BeveledCurve : ParametricSurface {
  BeveledCurve(BSDF *mat,
               float radius, // Radius of circle
               int tCount,    // Number of divisions along curve
               int rCount,    // Number of divisions of circle
               float tMin,   // min t value for curve
               float tMax    // max t value for curve
               )
      : ParametricSurface(mat, tCount, rCount, tMin, tMax, 0, TAU),
        radius(radius) {}

  Vec3 P(float a, float b);

  // Curve to bevel
  virtual Vec3 F(float t) = 0;

private:
  float radius;
};

struct LineBevel : BeveledCurve {
  LineBevel(BSDF *mat,
            float radius = 1, // Radius of circle
            int tCount = 10,   // Number of divisions along curve
            int rCount = 20,   // Number of divisions of circle
            float tMin = -1,  // min and max parameters for curve
            float tMax = 1
            )
      : BeveledCurve(mat, radius, tCount, rCount, tMin, tMax) {}

  Vec3 F(float t) { return Vec3(t, 0, 0); }
};

struct TorusBevel : BeveledCurve {
  TorusBevel(BSDF *mat,
             float radius = 1, // Radius of circle
             int tCount = 10,   // Number of divisions along curve
             int rCount = 20,   // Number of divisions of circle
             float tMin = 0,   // min and max parameters for curve
             float tMax = 2 * PI
             )
      : BeveledCurve(mat, radius, tCount, rCount, tMin, tMax) {}

  Vec3 F(float t) { return Vec3(cos(t), sin(t), 0) * 3; }
};

struct SpiralBevel : BeveledCurve {
  SpiralBevel(BSDF *mat,
              float radius = .4, // Radius of circle
              int tCount = 100,   // Number of divisions along curve
              int rCount = 20,    // Number of divisions of circle
              float tMin = 0,    // min and max parameters for curve
              float tMax = 8 * PI, 
              float heightScale = 1.0 / 6.0
              )
      : BeveledCurve(mat, radius, tCount, rCount, tMin, tMax), 
        h(heightScale) {}

  Vec3 F(float t) { return Vec3(cos(t), sin(t), t * h) * 3; }
  float h;
};

/**
 * Sourced from:
 * https://www.maa.org/sites/default/files/images/upload_library/23/stemkoski/knots/page5.html#:~:text=Parametric%20equations%20for%20such%20a,generality)%20that%20p%20%3E%20q%20.
 */
struct TorusKnotBevel : BeveledCurve {
  TorusKnotBevel(BSDF *mat,
                 float radius = .05, // Radius of circle
                 int P = 6,           // P for Torus Knot (prime)
                 int Q = 11,          // Q for torus knot (prime)
                 float K = 2,        // Magic parametrization constant
                 int tCount = 10000,  // Number of divisions along curve
                 int rCount = 20,     // Number of divisions of circle
                 float tMin = 0,     // min and max parameters for curve
                 float tMax = 3 * PI
                 )
      : BeveledCurve(mat, radius, tCount, rCount, tMin, tMax), 
        P(P), Q(Q), K(K) {}

  Vec3 F(float t) {
    return Vec3(
      cos(Q * t) * (K + cos(P * t)), 
      sin(Q * t) * (K + cos(P * t)),
      sin(P * t)
    );
  }
  int P;
  int Q;
  float K;
};

struct ButterflyBevel : BeveledCurve {
  ButterflyBevel(BSDF *mat,
                 int tCount = 1000,   // Number of divisions along curve
                 int rCount = 20,     // Number of divisions of circle
                 float radius = .15, // Radius of circle
                 float tMin = 0,     // min and max parameters for curve
                 float tMax = 12
                 )
      : BeveledCurve(mat, radius, tCount, rCount, tMin, tMax) {}

  Vec3 F(float t) {
    float coef = exp(cos(t)) - 2 * cos(4 * t) - pow(sin(t / 12), 5);
    return Vec3(coef * sin(t), coef * cos(t), t / 6.0);
  }
};

