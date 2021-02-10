#include "core/camera.h"
#include "core/math.h"


Camera::Camera(Vec e, Vec g, Vec up, double f, RenderParams& params) :
               e(e), f(f) {
  w = norm(-g);
  u = norm(cross(w, up));
  v = norm(cross(u, w));

  C2W = Matrix(u.x, v.x, w.x, e.x,
               u.y, v.y, w.y, e.y,
               u.z, v.z, w.z, e.z,
                 0,   0,   0,   1);
  W2C = invert(C2W);

  // TODO: Separate X/Y
  int sx = params.getInt("resolution");
  int sy = params.getInt("resolution");
  // TODO: Compute ratio w.r.t X/Y
  wsize = 4, wl = -2, wt =  2;

  du =  wsize / (sx - 1);
  dv = -wsize / (sy - 1);
}

Ray Camera::getRay(int i, int j, RNG& rng) {
  Vec pij = Vec(wl + (i + rng.rand01())*du, wt + (j + rng.rand01())*dv, f);
  Vec p = C2W * pij;
  return Ray(p, norm(p - e));
}

Ray Camera::getRay(int i, int j) {
  Vec pij = Vec(wl + (i)*du, wt + (j)*dv, f);
  Vec p = C2W * pij;
  return Ray(p, norm(p - e));
}
