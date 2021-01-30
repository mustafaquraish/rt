
#include "objects/parametric_surface.h"

using namespace std;

Vec ParametricSurface::N(double a, double b) {
  double EPS = 10e-4;
  Vec p_ab = P(a, b);
  Vec ta = norm((P(a+EPS, b) - p_ab) / EPS);
  Vec tb = norm((P(a, b+EPS) - p_ab) / EPS);
  return cross(ta, tb);
}

Vec ParametricSurface::T(double a, double b) {
  return Vec(
    map(a, aMin, aMax, 0, 1),
    map(b, bMin, bMax, 0, 1),
    0
  );
}

void ParametricSurface::createSurface() {
  double d_a = (aMax - aMin) / (double)aCount;
  double d_b = (bMax - bMin) / (double)bCount;

  std::vector<Primitive *> tris_list;

  for (int i = 0; i < aCount; i++) {
    double a1 = aMin + i * d_a;
    double a2 = a1 + d_a;

    for (int j = 0; j < bCount; j++) {
      double b1 = bMin + j * d_b;
      double b2 = b1 + d_b;

      // Get current circle points / normals./ tex coords..
      //   names represent p_ab, n_ab and t_ab
      Vec p_11 = P(a1, b1); Vec n_11 = N(a1, b1); Vec t_11 = T(a1, b1);
      Vec p_12 = P(a1, b2); Vec n_12 = N(a1, b2); Vec t_12 = T(a1, b2);

      Vec p_21 = P(a2, b1); Vec n_21 = N(a2, b1); Vec t_21 = T(a2, b1);
      Vec p_22 = P(a2, b2); Vec n_22 = N(a2, b2); Vec t_22 = T(a2, b2);
      
      Triangle *tri1 = new Triangle(p_11, p_12, p_22,
                                    n_11, n_12, n_22,
                                    t_11, t_12, t_22);

      Triangle *tri2 = new Triangle(p_11, p_22, p_21,
                                    n_11, n_22, n_21,
                                    t_11, t_22, t_21);

      tris_list.push_back(tri1);
      tris_list.push_back(tri2);
    }
  }
  loadTriangles(tris_list);
}