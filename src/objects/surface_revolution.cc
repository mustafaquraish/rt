#include "objects/surface_revolution.h"

using namespace std;

/* Define P(a, b) for ParametricSurface to use based on F */
Vec SurfaceOfRevolution::P(double a, double b) {
  return RotateYMatrix(b) * F(a);
}