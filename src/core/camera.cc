#include "core/camera.h"
#include "core/math.h"


Camera::Camera(Vec e, Vec g, Vec up, double vfov, RenderParams& params) :
               e(e) {
  Vec w = norm(-g);
  Vec u = norm(cross(w, up));
  Vec v = norm(cross(u, w));

  C2W = Matrix(u.x, v.x, w.x, e.x,
               u.y, v.y, w.y, e.y,
               u.z, v.z, w.z, e.z,
                 0,   0,   0,   1);
  W2C = invert(C2W);

  // TODO: Separate X/Y
  int sx = params.get<int>("width");
  int sy = params.get<int>("height");

  if (params.exists("aperture") && params.exists("focus_dist")) {
    DOF = true;
    aperture = params.get<double>("aperture");
    focus_dist = params.get<double>("focus_dist");
  }

  double theta = radians(vfov);
  double ysize = 2 * tan(theta / 2.0);
  double xsize = ((double)sx / (double)sy) * ysize; 

  wl = -xsize / 2;
  wt =  ysize / 2;

  du =  xsize / (sx - 1);
  dv = -ysize / (sy - 1);
}

Ray Camera::getRay(int i, int j, RNG& rng) {
  if (!DOF) {
    // TODO: Why does Ray(e, norm(p-e)) break KD-Tree?
    Vec pij = Vec(wl + (i+rng.rand01())*du, 
                  wt + (j+rng.rand01())*dv, 
                  -1);
    Vec p = C2W * pij;
    return Ray(p, norm(p - e));
  
  } else {
    Vec pij = Vec(wl + i*du, wt + j*dv, -1);
    Vec aperture_point = pij + rng.randomUnitDisk() * aperture;
    Vec p = C2W * (norm(pij) * focus_dist);
    Vec o = C2W * aperture_point;
    return Ray(o, norm(p - o));
  }
}

// TODO: Why does Ray(e, norm(p-e)) break KD-Tree?
Ray Camera::getRay(int i, int j) {
  Vec pij = Vec(wl + (i)*du, wt + (j)*dv, -1);
  Vec p = C2W * pij;
  return Ray(p, norm(p - e));
}
