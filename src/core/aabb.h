#pragma once

#include <core/math.h>
#include <core/ray.h>

struct AABB {
  // Bounding box with Inifinite bounds
  AABB() : min(Vec3(INFINITY)), max(Vec3(-INFINITY)) {};
  // Bounding box from (-v, -v, -v) to (v, v, v)
  AABB(double v) : min(Vec3(-v)), max(Vec3(v)) {};
  // Create Bounds with a single point
  AABB(Vec3 v) : min(v), max(v) {};
  // Bounding box with `min` and `max` bounds
  AABB(Vec3 min, Vec3 max) : min(min), max(max) {};
  
  void addPoint(const Vec3& p);
  Vec3 offset(const Vec3& p) const;
  bool hit(Ray &ray, double &t1, double &t2, const Vec3& invD);
  bool hit(Ray &ray, double &t1, double &t2);
  bool hit(Ray &ray);
  Vec3 min;
  Vec3 max;
};

std::ostream& operator<<(std::ostream& os, const AABB& bb);

AABB combine(const AABB& a, const AABB& b);
AABB transformed(const Matrix4& T, const AABB& b);
bool overlap(const AABB& a, const AABB& b);
Vec3 centroid(const AABB& a);
double area(const AABB& a);

