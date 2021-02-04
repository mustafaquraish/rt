
/**
 * A surface of revolution rCount itself into a mesh.
 */

#ifndef __BEVEL_CURVE_H__
#define __BEVEL_CURVE_H__

#include "objects/parametric_surface.h"

/**
 * A Beveled Curve is just a parametric surface where parameter `a`
 *  is a custom function, and the parameter `b` is reponsible for a point
 *  on a circle around the curve
 */
struct BeveledCurve : ParametricSurface {
  BeveledCurve(BSDF *mat,
               double radius, // Radius of circle
               int tCount,    // Number of divisions along curve
               int rCount,    // Number of divisions of circle
               double tMin,   // min t value for curve
               double tMax    // max t value for curve
               )
      : ParametricSurface(mat, tCount, rCount, tMin, tMax, 0, TAU),
        radius(radius) {}

  Vec P(double a, double b);

  // Curve to bevel
  virtual Vec F(double t) = 0;

private:
  double radius;
};

struct LineBevel : BeveledCurve {
  LineBevel(BSDF *mat,
            double radius = 1, // Radius of circle
            int tCount = 10,   // Number of divisions along curve
            int rCount = 20,   // Number of divisions of circle
            double tMin = -1,  // min and max parameters for curve
            double tMax = 1
            )
      : BeveledCurve(mat, radius, tCount, rCount, tMin, tMax) {}

  Vec F(double t) { return Vec(t, 0, 0); }
};

struct TorusBevel : BeveledCurve {
  TorusBevel(BSDF *mat,
             double radius = 1, // Radius of circle
             int tCount = 10,   // Number of divisions along curve
             int rCount = 20,   // Number of divisions of circle
             double tMin = 0,   // min and max parameters for curve
             double tMax = 2 * PI
             )
      : BeveledCurve(mat, radius, tCount, rCount, tMin, tMax) {}

  Vec F(double t) { return Vec(cos(t), sin(t), 0) * 3; }
};

struct SpiralBevel : BeveledCurve {
  SpiralBevel(BSDF *mat,
              double radius = .4, // Radius of circle
              int tCount = 100,   // Number of divisions along curve
              int rCount = 20,    // Number of divisions of circle
              double tMin = 0,    // min and max parameters for curve
              double tMax = 8 * PI, 
              double heightScale = 1.0 / 6.0
              )
      : BeveledCurve(mat, radius, tCount, rCount, tMin, tMax), 
        h(heightScale) {}

  Vec F(double t) { return Vec(cos(t), sin(t), t * h) * 3; }
  double h;
};

/**
 * Sourced from:
 * https://www.maa.org/sites/default/files/images/upload_library/23/stemkoski/knots/page5.html#:~:text=Parametric%20equations%20for%20such%20a,generality)%20that%20p%20%3E%20q%20.
 */
struct TorusKnotBevel : BeveledCurve {
  TorusKnotBevel(BSDF *mat,
                 double radius = .05, // Radius of circle
                 int P = 6,           // P for Torus Knot (prime)
                 int Q = 11,          // Q for torus knot (prime)
                 double K = 2,        // Magic parametrization constant
                 int tCount = 10000,  // Number of divisions along curve
                 int rCount = 20,     // Number of divisions of circle
                 double tMin = 0,     // min and max parameters for curve
                 double tMax = 3 * PI
                 )
      : BeveledCurve(mat, radius, tCount, rCount, tMin, tMax), 
        P(P), Q(Q), K(K) {}

  Vec F(double t) {
    return Vec(
      cos(Q * t) * (K + cos(P * t)), 
      sin(Q * t) * (K + cos(P * t)),
      sin(P * t)
    );
  }
  int P;
  int Q;
  double K;
};

struct ButterflyBevel : BeveledCurve {
  ButterflyBevel(BSDF *mat,
                 int tCount = 1000,   // Number of divisions along curve
                 int rCount = 20,     // Number of divisions of circle
                 double radius = .15, // Radius of circle
                 double tMin = 0,     // min and max parameters for curve
                 double tMax = 12
                 )
      : BeveledCurve(mat, radius, tCount, rCount, tMin, tMax) {}

  Vec F(double t) {
    double coef = exp(cos(t)) - 2 * cos(4 * t) - pow(sin(t / 12), 5);
    return Vec(coef * sin(t), coef * cos(t), t / 6.0);
  }
};

#endif // __BEVEL_CURVE_H__
