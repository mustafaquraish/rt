
#include <objects/bevel_curve.h>

using namespace std;

/* Define P(a, b) for ParametricSurface to use based on F */
Vec3 BeveledCurve::P(float a, float b) {

  Vec3 curve_point = F(a);
  Vec3 curve_tangent = normalized((F(a + EPS) - curve_point) / EPS);
  
  Vec3 circle_point = Vec3(cos(b), sin(b), 0) * radius;

  return align_to(circle_point, curve_tangent) + curve_point;
}