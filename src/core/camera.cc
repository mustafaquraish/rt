#include <core/camera.h>
#include <core/rng.h>
#include <util/params.h>

Camera::Camera(Vec3 e, Vec3 g, Vec3 up, float vfov, RenderParams& params) :
               e(e) {
  Vec3 w = normalized(-g);
  Vec3 u = normalized(cross(w, up));
  Vec3 v = normalized(cross(u, w));

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
    aperture = params.get<float>("aperture");
    focus_dist = params.get<float>("focus_dist");
  }

  float theta = radians(vfov);
  float ysize = 2 * tan(theta / 2.0);
  float xsize = ((float)sx / (float)sy) * ysize; 

  wl = -xsize / 2;
  wt =  ysize / 2;

  du =  xsize / (sx - 1);
  dv = -ysize / (sy - 1);
}

Ray Camera::getRay(int i, int j, RNG& rng) {
  if (!DOF) {
    // TODO: Why does Ray(e, normalized(p-e)) break KD-Tree?
    Vec3 pij = Vec3(wl + (i + rng.rand01()) * du,
                    wt + (j + rng.rand01()) * dv,
                    -1);
    Vec3 p = C2W * pij;
    return Ray(p, normalized(p - e));

  } else {
    Vec3 pij = Vec3(wl + i * du, wt + j * dv, -1);
    Vec3 aperture_point = pij + rng.randomUnitDisk() * aperture;
    Vec3 p = C2W * (normalized(pij) * focus_dist);
    Vec3 o = C2W * aperture_point;
    return Ray(o, normalized(p - o));
  }
}

// TODO: Why does Ray(e, normalized(p-e)) break KD-Tree?
Ray Camera::getRay(int i, int j) {
  Vec3 pij = Vec3(wl + (i) * du, wt + (j) * dv, -1);
  Vec3 p = C2W * pij;
  return Ray(p, normalized(p - e));
}
