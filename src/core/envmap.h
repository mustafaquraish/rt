#ifndef __ENVMAP_H__
#define __ENVMAP_H__

#include "core/object.h"
#include "materials/emitter.h"

/* Placeholder object so that we can use the existing NEE framework */
struct SkyObject : Object {
  SkyObject(BSDF *mat, double brightness) : Object(mat) { 
    bounds = AABB(0);   
    surfaceArea = brightness;
  };
  bool hit(Ray& r, HitRec &rec) { 
    rec.obj = this;
    rec.t = 1;
    Vec it = norm(r.d);
    rec.n = -it;
    rec.u = atan2(it.z, it.x) / (2 * PI) + 0.5;
    rec.v = 0.5 - asin(it.y) / PI;
    return true; 
  };

  Vec sample(double *pdf, RNG& rng) {
    double theta = 2 * PI * rng.rand01();
    double phi = acos(2 * rng.rand01() - 1);
    *pdf = 1.0 / surfaceArea;

    // Multiply by large constant to move it away from scene
    return 1000 * polar2Cart(theta, phi);
  }
};

struct EnvironmentMap : ImageTexture {
  EnvironmentMap(const char *filename, double brightness=10) 
    : ImageTexture(filename) {
    envObject = new SkyObject(new Emitter(this), brightness);
  }
  virtual Colour get(HitRec& rec) { 
    Vec d = norm(rec.wo);
    double u = atan2(d.z, d.x) / (2 * PI) + 0.5;
    double v = 0.5 - asin(d.y) / PI;
    return im->get(u, v); 
  }

  Object *envObject;
};

#endif // __ENVMAP_H__
