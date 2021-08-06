#include <core/camera.h>
#include <core/math.h>


Camera::Camera(Vec3 e, Vec3 g, Vec3 up, double vfov, RenderParams& params) :
               e(e) {
  Vec3 w = norm(-g);
  Vec3 u = norm(cross(w, up));
  Vec3 v = norm(cross(u, w));

  C2W = Matrix4(u.x, v.x, w.x, e.x,
                u.y, v.y, w.y, e.y,
                u.z, v.z, w.z, e.z,
                0, 0, 0, 1);
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
    Vec3 pij = Vec3(wl + (i + rng.rand01()) * du,
                  wt + (j+rng.rand01())*dv,
                    -1);
    Vec3 p = C2W * pij;
    return Ray(p, norm(p - e));
  
  } else {
    Vec3 pij = Vec3(wl + i * du, wt + j * dv, -1);
    Vec3 aperture_point = pij + rng.randomUnitDisk() * aperture;
    Vec3 p = C2W * (norm(pij) * focus_dist);
    Vec3 o = C2W * aperture_point;
    return Ray(o, norm(p - o));
  }
}

// TODO: Why does Ray(e, norm(p-e)) break KD-Tree?
Ray Camera::getRay(int i, int j) {
  Vec3 pij = Vec3(wl + (i) * du, wt + (j) * dv, -1);
  Vec3 p = C2W * pij;
  return Ray(p, norm(p - e));
}
