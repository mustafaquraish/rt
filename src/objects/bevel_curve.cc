
#include "objects/bevel_curve.h"

using namespace std;

Vec BeveledCurve::T(double t) {
  double EPS = 10e-4;
  Vec tangent = norm((F(t+EPS) - F(t)) / EPS);
  return tangent;
}

// `t` goes from 0 -> 2PI
Vec BeveledCurve::getCirclePoint(double t) {
  return Vec(cos(t), sin(t), 0) * radius;
}

void BeveledCurve::createSurface() {
  double dt = (tMax - tMin) / (double)loops;
  double dtheta = 2*PI/(double)turns;

  std::vector<Primitive *> tris_list;

  for (int i = 0; i < loops; i++) {
    double t1 = tMin + i * dt;
    double t2 = tMin + (i+1) * dt;

    Vec p1 = F(t1), p2 = F(t2);
    Vec tan_1 = T(t1), tan_2 = T(t2);

    // Matrix to align circle with the curve at point p1 and p2
    Matrix m1 = TranslateMatrix(p1.x, p1.y, p1.z) * getRotationMatrix(tan_1); 
    Matrix m2 = TranslateMatrix(p2.x, p2.y, p2.z) * getRotationMatrix(tan_2); 

    for (int j = 0; j < turns; j++) {
      double a1 = j * dtheta, a2 = (j+1) * dtheta;

      // Get current circle points...
      Vec circ_1 = getCirclePoint(a1);
      Vec circ_2 = getCirclePoint(a2);

      Vec a = m1 * circ_1; Vec na = p1 - a;
      Vec b = m1 * circ_2; Vec nb = p1 - b;

      Vec c = m2 * circ_1; Vec nc = p2 - c;
      Vec d = m2 * circ_2; Vec nd = p2 - d;
      
      // tris_list.push_back(new Triangle(a, b, c));
      // tris_list.push_back(new Triangle(c, b, d));
      tris_list.push_back(new Triangle(a, b, c, na, nb, nc));
      tris_list.push_back(new Triangle(c, b, d, nc, nb, nd));
    }
  }
  loadTriangles(tris_list);
}