#include <objects/surface_revolution.h>

using namespace std;

/* Define P(a, b) for ParametricSurface to use based on F */
Vec3 SurfaceOfRevolution::P(float a, float b) {
  return RotateYMatrix(b) * F(a);
}