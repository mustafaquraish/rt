
#include <objects/parametric_surface.h>

using namespace std;

Vec3 ParametricSurface::N(float a, float b) {
  float a2 = a + EPS;
  float b2 = b + EPS;

  Vec3 pab = P(a, b);
  Vec3 pa2b = P(a2, b );
  Vec3 pab2 = P(a, b2 );

  Vec3 da = (pa2b - pab) / EPS;
  Vec3 db = (pab2 - pab) / EPS;

  Vec3 ta = normalized(da);
  Vec3 tb = normalized(db);

  if (isnan(ta.x) || isnan(tb.x)) {
    printf("got nan in parametric surface normal\n");
    Vec3 p1_ab = P(a, b);
    Vec3 p1a2b = P(a2, b );
    Vec3 p1ab2 = P(a, b2 );
  }

  return cross(tb, ta);
}

Vec2 ParametricSurface::T(float a, float b) {
  return Vec2(
    map(a, aMin, aMax, 0, 1),
    map(b, bMin, bMax, 0, 1)
  );
}

void ParametricSurface::finalize() {
  auto prims = createSurface();
  loadTriangles(prims);
  Object::finalize();
}

std::vector<Primitive *> ParametricSurface::createSurface() {
  float d_a = (aMax - aMin) / (float)aCount;
  float d_b = (bMax - bMin) / (float)bCount;

  std::vector<Primitive *> tris_list;

  for (int i = 0; i < aCount; i++) {
    float a1 = aMin + d_a * i;
    float a2 = aMin + d_a * (i + 1);

    for (int j = 0; j < bCount; j++) {
      float b1 = bMin + d_b * j;
      float b2 = bMin + d_b * (j + 1);

      // Get current circle points / normals./ tex coords..
      //   names represent p_ab, n_ab and t_ab
      Vec3 p_11 = P(a1, b1); Vec2 t_11 = T(a1, b1);
      Vec3 p_12 = P(a1, b2); Vec2 t_12 = T(a1, b2);

      Vec3 p_21 = P(a2, b1); Vec2 t_21 = T(a2, b1);
      Vec3 p_22 = P(a2, b2); Vec2 t_22 = T(a2, b2);
      
      Triangle *t1, *t2;
      if (interpolateNormals) {
        Vec3 n_11 = N(a1, b1), n_12 = N(a1, b2);
        Vec3 n_21 = N(a2, b1), n_22 = N(a2, b2);
        t1 = new Triangle(p_11, p_12, p_22, n_11, n_12, n_22, t_11, t_12, t_22);
        t2 = new Triangle(p_11, p_22, p_21, n_11, n_22, n_21, t_11, t_22, t_21);
      } else {
        Vec3 n1 = cross(p_12 - p_11, p_22 - p_11);
        Vec3 n2 = cross(p_22 - p_11, p_21 - p_11);
        t1 = new Triangle(p_11, p_12, p_22, n1, n1, n1, t_11, t_12, t_22);
        t2 = new Triangle(p_11, p_22, p_21, n2, n2, n2, t_11, t_22, t_21);
      }

      tris_list.push_back(t1);
      tris_list.push_back(t2);
    }
  }
  return tris_list;
}