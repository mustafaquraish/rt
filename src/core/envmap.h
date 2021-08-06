#pragma once

#include <core/object.h>
#include <materials/emitter.h>

struct EnvironmentMap : Object {
  EnvironmentMap(Texture *tx, float brightness=10) 
    : Object(new Emitter(tx)) { 
    bounds = AABB(0);   
    surfaceArea = brightness;
  }

  EnvironmentMap(const char *filename, float brightness=10) 
    : EnvironmentMap(new ImageTexture(filename), brightness) {}

  bool hit(Ray& r, HitRec &rec) { 
    Ray ray = rayTransform(r);
    rec.obj = this;
    rec.t = 1;
    Vec3 it = normalized(ray.d);
    rec.n = -it;
    rec.u = atan2(it.z, it.x) / (2 * PI) + 0.5;
    rec.v = 0.5 + asin(it.y) / PI;
    return true; 
  };

  Vec3 sample(float *pdf, RNG& rng) {
    float theta = 2 * PI * rng.rand01();
    float phi = acos(2 * rng.rand01() - 1);
    *pdf = 1.0 / surfaceArea;

    // Multiply by large constant to move it away from scene
    return 1000 * from_polar(theta, phi);
  }
};

