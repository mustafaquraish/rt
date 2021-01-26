#ifndef __AABB_H__
#define __AABB_H__

#include "core/math.h"
#include "core/ray.h"

struct AABB {
  // Bounding box with Inifinite bounds
  AABB() : min(Vec(-INFINITY)), max(Vec(INFINITY)) {};
  // Bounding box from (-v, -v, -v) to (v, v, v)
  AABB(double v) : min(Vec(-v)), max(Vec(v)) {};
  // Create Bounds with a single point
  AABB(Vec v) : min(v), max(v) {};
  // Bounding box with `min` and `max` bounds
  AABB(Vec min, Vec max) : min(min), max(max) {};
  
  void addPoint(const Vec& p);
  bool hit(Ray &ray, double &t_min, double &t_max);
  bool hit(Ray &ray);
  Vec min;
  Vec max;
};

std::ostream& operator<<(std::ostream& os, const AABB& bb);

AABB combine(const AABB& a, const AABB& b);
AABB transformed(const Matrix& T, const AABB& b);
Vec centroid(const AABB& a);

#endif // __AABB_H__
