#include <core/aabb.h>
#include <core/ray.h>

// Slightly faster AABB intersection when 1 / ray.d is given.
bool AABB::hit(Ray& ray, float &t1, float &t2, const Vec3& invD) {
  float tmin = -INFINITY;
  float tmax = INFINITY;

  // Refine using slabs along each axis
  for (int axis = 0; axis < 3; axis++) {
    float _tmin = (min[axis] - ray.p[axis]) * invD[axis];
    float _tmax = (max[axis] - ray.p[axis]) * invD[axis];
    if (_tmin > _tmax) std::swap(_tmin, _tmax); 
    if (tmin > _tmax || _tmin > tmax) return false;
    if (_tmin > tmin) tmin = _tmin;
    if (_tmax < tmax) tmax = _tmax;
  }

  if (tmin > ray.tMax) return false;
  if (tmin < TOL && tmax < TOL) return false;
  if (tmin < TOL) tmin = 0;

  t1 = tmin;
  t2 = tmax;
  return true;
}

bool AABB::hit(Ray& ray, float &t1, float &t2) {
  float tmin = -INFINITY;
  float tmax = INFINITY;

  // Refine using slabs along each axis
  for (int axis = 0; axis < 3; axis++) {
    float _tmin = (min[axis] - ray.p[axis]) / ray.d[axis];
    float _tmax = (max[axis] - ray.p[axis]) / ray.d[axis];
    if (_tmin > _tmax) std::swap(_tmin, _tmax); 
    if (tmin > _tmax || _tmin > tmax) return false;
    if (_tmin > tmin) tmin = _tmin;
    if (_tmax < tmax) tmax = _tmax;
  }

  if (tmin > ray.tMax) return false;
  if (tmin < TOL && tmax < TOL) return false;
  if (tmin < TOL) tmin = 0;

  t1 = tmin;
  t2 = tmax;
  return true;
}

bool AABB::hit(Ray& ray) {
  float t1, t2; // These will be discarded
  return hit(ray, t1, t2);
}

AABB combine(const AABB& a, const AABB& b) {
  return AABB(
      Vec3(min(a.min.x, b.min.x), min(a.min.y, b.min.y), min(a.min.z, b.min.z)),
      Vec3(max(a.max.x, b.max.x), max(a.max.y, b.max.y), max(a.max.z, b.max.z))
  );
}

AABB transformed(const Matrix4& T, const AABB& b) {
  Vec3 pts[8] = {
      T * Vec3(b.min.x, b.min.y, b.min.z),
      T * Vec3(b.min.x, b.min.y, b.max.z),
      T * Vec3(b.min.x, b.max.y, b.min.z),
      T * Vec3(b.min.x, b.max.y, b.max.z),
      T * Vec3(b.max.x, b.min.y, b.min.z),
      T * Vec3(b.max.x, b.min.y, b.max.z),
      T * Vec3(b.max.x, b.max.y, b.min.z),
      T * Vec3(b.max.x, b.max.y, b.max.z),
  };
  AABB tBounds = AABB(pts[0]);
  for (int i = 0; i < 8; i++) {
    tBounds.addPoint(pts[i]);
  }
  return tBounds;
}

void AABB::addPoint(const Vec3& p) {
  min.x = fmin(min.x, p.x);
  min.y = fmin(min.y, p.y);
  min.z = fmin(min.z, p.z);

  max.x = fmax(max.x, p.x);
  max.y = fmax(max.y, p.y);
  max.z = fmax(max.z, p.z);
}

Vec3 AABB::offset(const Vec3& p) const {
  Vec3 t = p - min;
  if (max.x > min.x) t.x /= max.x - min.x;
  if (max.y > min.y) t.y /= max.y - min.y;
  if (max.z > min.z) t.z /= max.z - min.z;
  return t;
}

float area(const AABB& a) {
  Vec3 d = a.max - a.min;
  return 2 * (d.x * d.y + d.x * d.z + d.y * d.z);
}

Vec3 centroid(const AABB& a) {
  return (a.min + a.max) / 2;
}

Vec3 range(const AABB& a) {
  return (a.max - a.min);
}

bool overlap(const AABB& a, const AABB& b) {
  bool x = (a.max.x >= b.min.x) && (a.min.x <= b.max.x);
  bool y = (a.max.y >= b.min.y) && (a.min.y <= b.max.y);
  bool z = (a.max.z >= b.min.z) && (a.min.z <= b.max.z);
  return (x && y && z);
}

std::ostream& operator<<(std::ostream& os, const AABB& bb) {
  os << "[(" << bb.min << ") -> (" << bb.max << ")]";
  return os;
}