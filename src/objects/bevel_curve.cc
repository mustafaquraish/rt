
#include "objects/bevel_curve.h"

using namespace std;

/* Define P(a, b) for ParametricSurface to use based on F */
Vec BeveledCurve::P(double a, double b) {
  double EPS = 10e-4;

  Vec curve_point = F(a);
  Vec curve_tangent = norm((F(a+EPS) - curve_point) / EPS);
  
  Vec circle_point = Vec(cos(b), sin(b), 0) * radius;

  return alignTo(circle_point, curve_tangent) + curve_point;
}