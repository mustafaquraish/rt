
#include "objects/parametric_surface.h"

using namespace std;

Vec ParametricSurface::N(double a, double b) {
  double a2 = a + EPS;
  double b2 = b + EPS;

  Vec pab = P(a, b);
  Vec pa2b = P(a2, b );
  Vec pab2 = P(a, b2 );

  Vec da = (pa2b - pab) / EPS;
  Vec db = (pab2 - pab) / EPS;

  Vec ta = norm(da);
  Vec tb = norm(db);

  if (isnan(ta.x) || isnan(tb.x)) {
    printf("got nan in parametric surface normal\n");
    Vec p1_ab = P(a, b);
    Vec p1a2b = P(a2, b );
    Vec p1ab2 = P(a, b2 );
  }

  return cross(tb, ta);
}

Vec ParametricSurface::T(double a, double b) {
  return Vec(
    map(a, aMin, aMax, 0, 1),
    map(b, bMin, bMax, 0, 1),
    0
  );
}

void ParametricSurface::finalize() {
  auto prims = createSurface();
  loadTriangles(prims);
  Object::finalize();
}

std::vector<Primitive *> ParametricSurface::createSurface() {
  double d_a = (aMax - aMin) / (double)aCount;
  double d_b = (bMax - bMin) / (double)bCount;

  std::vector<Primitive *> tris_list;

  for (int i = 0; i < aCount; i++) {
    double a1 = aMin + d_a * i;
    double a2 = aMin + d_a * (i + 1);

    for (int j = 0; j < bCount; j++) {
      double b1 = bMin + d_b * j;
      double b2 = bMin + d_b * (j + 1);

      // Get current circle points / normals./ tex coords..
      //   names represent p_ab, n_ab and t_ab
      Vec p_11 = P(a1, b1); Vec t_11 = T(a1, b1);
      Vec p_12 = P(a1, b2); Vec t_12 = T(a1, b2);

      Vec p_21 = P(a2, b1); Vec t_21 = T(a2, b1);
      Vec p_22 = P(a2, b2); Vec t_22 = T(a2, b2);
      
      Triangle *t1, *t2;
      if (interpolateNormals) {
        Vec n_11 = N(a1, b1), n_12 = N(a1, b2); 
        Vec n_21 = N(a2, b1), n_22 = N(a2, b2);
        t1 = new Triangle(p_11, p_12, p_22, n_11, n_12, n_22, t_11, t_12, t_22);
        t2 = new Triangle(p_11, p_22, p_21, n_11, n_22, n_21, t_11, t_22, t_21);
      } else {
        Vec n1 = cross(p_12 - p_11, p_22 - p_11);
        Vec n2 = cross(p_22 - p_11, p_21 - p_11);
        t1 = new Triangle(p_11, p_12, p_22, n1, n1, n1, t_11, t_12, t_22);
        t2 = new Triangle(p_11, p_22, p_21, n2, n2, n2, t_11, t_22, t_21);
      }

      tris_list.push_back(t1);
      tris_list.push_back(t2);
    }
  }
  return tris_list;
}