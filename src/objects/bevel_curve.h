
/**
 * A surface of revolution turns itself into a mesh.
 */

#ifndef __BEVEL_CURVE_H__
#define __BEVEL_CURVE_H__

#include "objects/triangle_mesh.h"

struct BeveledCurve : TriangleMesh {
  BeveledCurve(Material *mat,
               int loops,     // Number of divisions along curve
               int turns,     // Number of divisions of circle
               double radius, // Radius of circle
               double tMin,   // min and max parameters for curve
               double tMax)
      : TriangleMesh(mat), loops(loops), turns(turns), radius(radius),
        tMin(tMin), tMax(tMax) {}

  void createSurface();

  // Function to rotate around Y axis
  virtual Vec F(double t) = 0;
  void finalize() {
    createSurface();
    Object::finalize();
  }

  int loops;
  int turns;
  double radius;
  double tMin;
  double tMax;

private:
  // Numerically compute tangent
  Vec T(double t);
  Vec getCirclePoint(double t);
};

struct LineBevel : BeveledCurve {
  LineBevel(Material *mat,
             int loops = 10,    // Number of divisions along curve
             int turns = 20,    // Number of divisions of circle
             double radius = 1, // Radius of circle
             double tMin = -1,   // min and max parameters for curve
             double tMax =  1)
      : BeveledCurve(mat, loops, turns, radius, tMin, tMax) {}

  Vec F(double t) { 
    return Vec(t, 0, 0); 
  }
};

struct TorusBevel : BeveledCurve {
  TorusBevel(Material *mat,
             int loops = 10,    // Number of divisions along curve
             int turns = 20,    // Number of divisions of circle
             double radius = 1, // Radius of circle
             double tMin = 0,   // min and max parameters for curve
             double tMax = 2*PI)
      : BeveledCurve(mat, loops, turns, radius, tMin, tMax) {}

  Vec F(double t) { 
    return Vec(cos(t), sin(t), 0) * 3; 
  }
};

struct SpiralBevel : BeveledCurve {
  SpiralBevel(Material *mat,
              int loops = 100,    // Number of divisions along curve
              int turns = 20,     // Number of divisions of circle
              double radius = .4, // Radius of circle
              double tMin = 0,    // min and max parameters for curve
              double tMax = 8 * PI, 
              double heightScale = 1.0 / 6.0)
      : BeveledCurve(mat, loops, turns, radius, tMin, tMax), h(heightScale) {}

  Vec F(double t) { 
    return Vec(cos(t), sin(t), t * h) * 3; 
  }
  double h;
};

/**
 * Sourced from:
 * https://www.maa.org/sites/default/files/images/upload_library/23/stemkoski/knots/page5.html#:~:text=Parametric%20equations%20for%20such%20a,generality)%20that%20p%20%3E%20q%20.
 */
struct TorusKnotBevel : BeveledCurve {
  TorusKnotBevel(Material *mat,
                 int P = 3,           // P for Torus Knot (prime)
                 int Q = 5,           // Q for torus knot (prime)
                 double K = 2,        // Magic parametrization constant
                 int loops = 10000,   // Number of divisions along curve
                 int turns = 20,      // Number of divisions of circle
                 double radius = .05, // Radius of circle
                 double tMin = 0,     // min and max parameters for curve
                 double tMax = 3 * PI)
      : BeveledCurve(mat, loops, turns, radius, tMin, tMax), P(P), Q(Q), K(K) {}

  Vec F(double t) {
    return Vec(cos(Q * t) * (K + cos(P * t)), 
               sin(Q * t) * (K + cos(P * t)),
               sin(P * t));
  }
  int P;
  int Q;
  double K;
};

struct ButterflyBevel : BeveledCurve {
  ButterflyBevel(Material *mat,
                 int loops = 1000,   // Number of divisions along curve
                 int turns = 20,      // Number of divisions of circle
                 double radius = .15,  // Radius of circle
                 double tMin = 0,     // min and max parameters for curve
                 double tMax = 12)
      : BeveledCurve(mat, loops, turns, radius, tMin, tMax) {}

  Vec F(double t) {
    double coef = exp(cos(t)) - 2*cos(4*t) - pow(sin(t/12), 5);
    return Vec(coef * sin(t), coef * cos(t), t / 6.0);
  }
};

#endif // __BEVEL_CURVE_H__
