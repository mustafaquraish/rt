#include "objects/surface_revolution.h"

using namespace std;

Vec SurfaceOfRevolution::N(double t) {
  double EPS = 10e-4;
  Vec tangent = norm((F(t+EPS) - F(t)) / EPS);
  Vec normal = RotateZMatrix(PI/2) * tangent;
  return normal;
}

void SurfaceOfRevolution::createSurface() {
  double dt = (tMax - tMin) / (double)loops;
  double dtheta = 2*PI/(double)turns;
  std::vector<Primitive *> tris_list;
  bounds = AABB();
  for (int i = 0; i < loops; i++) {
    double t1 = tMin + i * dt;
    double t2 = tMin + (i+1) * dt;

    Vec p1 = F(t1), p2 = F(t2);
    Vec n1 = N(t1), n2 = N(t2);

    for (int j = 0; j < turns; j++) {
      double a1 = j * dtheta, a2 = (j+1) * dtheta;
      Matrix m1 = RotateYMatrix(a1), m2 = RotateYMatrix(a2); 

      Vec a = m1 * p1, na = m1 * n1;
      Vec b = m1 * p2, nb = m1 * n2;

      Vec c = m2 * p1, nc = m2 * n1;
      Vec d = m2 * p2, nd = m2 * n2;
      
      bounds.addPoint(a);
      bounds.addPoint(b);
      bounds.addPoint(c);
      bounds.addPoint(d);

      // tris_list.push_back(new Triangle(a, b, c));
      // tris_list.push_back(new Triangle(c, b, d));
      tris_list.push_back(new Triangle(a, b, c, na, nb, nc));
      tris_list.push_back(new Triangle(c, b, d, nc, nb, nd));
    }
  }
  loadTriangles(tris_list);
}