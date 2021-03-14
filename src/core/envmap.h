#ifndef __ENVMAP_H__
#define __ENVMAP_H__

#include "core/object.h"
#include "materials/emitter.h"

struct EnvironmentMap : Object {
  EnvironmentMap(Texture *tx, double brightness=10) 
    : Object(new Emitter(tx)) { 
    bounds = AABB(0);   
    surfaceArea = brightness;
  }

  EnvironmentMap(const char *filename, double brightness=10) 
    : EnvironmentMap(new ImageTexture(filename), brightness) {}

  bool hit(Ray& r, HitRec &rec) { 
    Ray ray = rayTransform(r);
    rec.obj = this;
    rec.t = 1;
    Vec it = norm(ray.d);
    rec.n = -it;
    rec.u = atan2(it.z, it.x) / (2 * PI) + 0.5;
    rec.v = 0.5 + asin(it.y) / PI;
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

#endif // __ENVMAP_H__
