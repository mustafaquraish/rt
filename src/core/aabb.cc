#include "core/aabb.h"

bool AABB::hit(Ray& ray, double &t_min, double &t_max) {
  double tmin = (min.x - ray.p.x) / ray.d.x;
  double tmax = (max.x - ray.p.x) / ray.d.x;

  if (tmin > tmax) {
    double temp = tmax;
    tmax = tmin;
    tmin = temp;
  }

  double tymin = (min.y - ray.p.y) / ray.d.y;
  double tymax = (max.y - ray.p.y) / ray.d.y;
  if (tymin > tymax) {
    double temp = tymax;
    tymax = tymin;
    tymin = temp;
  }

  if ((tmin > tymax) || (tymin > tmax))
    return false;

  if (tymin > tmin) tmin = tymin;
  if (tymax < tmax) tmax = tymax;

  double tzmin = (min.z - ray.p.z) / ray.d.z;
  double tzmax = (max.z - ray.p.z) / ray.d.z;
  if (tzmin > tzmax) {
    double temp = tzmax;
    tzmax = tzmin;
    tzmin = temp;
  }

  if ((tmin > tzmax) || (tzmin > tmax))
    return false;

  if (tzmin > tmin) tmin = tzmin;
  if (tzmax < tmax) tmax = tzmax;

  if (tmin < TOL && tmax < TOL) return false;

  if (tmin < TOL) tmin = 0;

  t_min = tmin;
  t_max = tmax;

  return true;
}

bool AABB::hit(Ray& ray) {
  double t1, t2; // These will be discarded
  return hit(ray, t1, t2);
}

AABB combine(const AABB& a, const AABB& b) {
  return AABB(
    Vec(min(a.min.x, b.min.x), min(a.min.y, b.min.y), min(a.min.z, b.min.z)),
    Vec(max(a.max.x, b.max.x), max(a.max.y, b.max.y), max(a.max.z, b.max.z))
  );
}

AABB transformed(const Matrix& T, const AABB& b) {
  Vec pts[8] = {
    T * Vec(b.min.x, b.min.y, b.min.z),
    T * Vec(b.min.x, b.min.y, b.max.z),
    T * Vec(b.min.x, b.max.y, b.min.z),
    T * Vec(b.min.x, b.max.y, b.max.z),
    T * Vec(b.max.x, b.min.y, b.min.z),
    T * Vec(b.max.x, b.min.y, b.max.z),
    T * Vec(b.max.x, b.max.y, b.min.z),
    T * Vec(b.max.x, b.max.y, b.max.z),
  };
  // AABB tBounds = AABB(0);
  AABB tBounds = AABB(pts[0]);
  for (int i = 0; i < 8; i++) {
    // std::cout << "pt: " << pts[i] << "  new bounds: " << tBounds << std::endl;
    tBounds.addPoint(pts[i]);
  }
  return tBounds;
}

void AABB::addPoint(const Vec& p) {
  min.x = fmin(min.x, p.x);
  min.y = fmin(min.y, p.y);
  min.z = fmin(min.z, p.z);

  max.x = fmax(max.x, p.x);
  max.y = fmax(max.y, p.y);
  max.z = fmax(max.z, p.z);
}

Vec AABB::offset(const Vec& p) const {
  Vec t = p - min;
  if (max.x > min.x) t.x /= max.x - min.x;
  if (max.y > min.y) t.y /= max.y - min.y;
  if (max.z > min.z) t.z /= max.z - min.z;
  return t;
}

double area(const AABB& a) {
  return 2 * lengthSq(a.max - a.min);
}

Vec centroid(const AABB& a) {
  return (a.min + a.max) / 2;
}

bool overlap(const AABB& a, const AABB& b) {
  return(a.min.x < b.max.x && b.min.x < a.max.x && 
         a.min.y < b.max.y && b.min.y < a.max.y &&
         a.min.z < b.max.z && b.min.z < a.max.z);
}

std::ostream& operator<<(std::ostream& os, const AABB& bb) {
  os << "[(" << bb.min << ") -> (" << bb.max << ")]";
  return os;
}