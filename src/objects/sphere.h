#pragma once

#include <core/object.h>

struct Sphere : Object {
  Sphere(BSDF *mat) : Object(mat) { 
    bounds = AABB(1);   
    surfaceArea = 4;
  };
  bool hit(Ray& r, HitRec &rec);
  Vec3 sample(float *pdf, RNG& rng);
};

