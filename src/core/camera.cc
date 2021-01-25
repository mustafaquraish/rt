#include "core/camera.h"
#include "core/math.h"

Camera::Camera(Vec e, Vec g, Vec up, double f, double wl, double wt,
               double wsize, int sx, int sy) :
               e(e), f(f), wl(wl), wt(wt), wsize(wsize) {
  w = norm(-g);
  u = norm(cross(w, up));
  v = norm(cross(u, w));

  C2W = Matrix(u.x, v.x, w.x, e.x,
               u.y, v.y, w.y, e.y,
               u.z, v.z, w.z, e.z,
                 0,   0,   0,   1);
  W2C = invert(C2W);

  du =  wsize / (sx - 1);
  dv = -wsize / (sy - 1);
}

Ray Camera::getRay(int i, int j) {
  Vec pij = Vec(wl + (i)*du, wt + (j)*dv, f);
  Vec p = C2W * pij;
  return Ray(p, norm(p - e));
}
