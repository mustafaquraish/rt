#pragma once
// clang-format off
#include <cmath>
#include <iomanip>
#include <iostream>

// Forward declarations
float min(const float a, const float b);
float max(const float a, const float b);
float clamp01(const float a);
float clamp(const float a, const float _min, const float _max);

/********************* 2D Vector *********************/

union Vec2 {
  float m_vals[2];
  struct { float x, y; };
  struct { float u, v; };

  Vec2()                  : x(0), y(0) {};
  Vec2(float v)           : x(v), y(v) {};
  Vec2(float x,  float y) : x(x), y(y) {};

  int num_dims() { return 2; }

  float &operator[] (int idx)       { return m_vals[idx]; }
  float  operator[] (int idx) const { return m_vals[idx]; };
};

inline std::ostream &operator<<(std::ostream &os, const Vec2 &t) {
  os << std::setprecision(3) << t.x << " " << t.y << " ";
  return os;
}

inline Vec2 operator+(const Vec2 &v1, const Vec2 &v2) { return Vec2(v1.x + v2.x, v1.y + v2.y); }
inline Vec2 operator+(const Vec2 &v, const float t) { return Vec2(v.x + t, v.y + t); }
inline Vec2 operator+(const float t, const Vec2 &v) { return Vec2(v.x + t, v.y + t); }

inline Vec2 operator-(const Vec2 &v1) { return Vec2(-v1.x, -v1.y); }
inline Vec2 operator-(const Vec2 &v1, const Vec2 &v2) { return Vec2(v1.x - v2.x, v1.y - v2.y); }
inline Vec2 operator-(const Vec2 &v, const float t) { return Vec2(v.x - t, v.y - t); }
inline Vec2 operator-(const float t, const Vec2 &v) { return t + (-v); }

inline Vec2 operator*(const Vec2 &a, const Vec2 &b) { return Vec2(a.x * b.x, a.y * b.y); }
inline Vec2 operator*(const float t, const Vec2 &v) { return Vec2(v.x * t, v.y * t); }
inline Vec2 operator*(const Vec2 &v, const float t) { return t * v; }

inline Vec2 operator/(const float t, const Vec2 &v) { return Vec2(t / v.x, t / v.y); }
inline Vec2 operator/(const Vec2 &v, const float t) { return Vec2(v.x / t, v.y / t); }

inline float dot(const Vec2 &a, const Vec2 &b) { return a.x * b.x + a.y * b.y; }
inline float length(const Vec2 &a) { return sqrt(dot(a, a)); }
inline float length_sq(const Vec2 &a) { return dot(a, a); }
inline Vec2 normalized(const Vec2 &a) { return a / length(a); }
inline Vec2 mult(const Vec2 &a, const Vec2 &b) { return a * b; }
inline Vec2 floor(const Vec2 &a) { return Vec2(floor(a.x), floor(a.y)); }
inline Vec2 sqrt(const Vec2 &v) { return Vec2(sqrt(v.x), sqrt(v.y)); }

inline Vec2 cross(const Vec2& a, const Vec2& b) { 
  return a.x * b.y - a.y * b.x; 
}

inline Vec2 &operator+=(Vec2 &a, const Vec2 &b) { return a = a + b; }
inline Vec2 &operator+=(Vec2 &a, const float &b) { return a = a + b; }

inline Vec2 &operator-=(Vec2 &a, const Vec2 &b) { return a = a - b; }
inline Vec2 &operator-=(Vec2 &a, const float b) { return a = a - b; }

inline Vec2 &operator*=(Vec2 &a, const Vec2 &b) { return a = a * b; }
inline Vec2 &operator*=(Vec2 &a, const float b) { return a = a * b; }

inline Vec2 &operator/=(Vec2 &a, const float b) { return a = a / b; }

inline Vec2 clamp01(const Vec2 &a) { return Vec2(clamp01(a.x), clamp01(a.y)); }
inline Vec2 clamp(const Vec2 &a, float mn, float mx) { return Vec2(clamp(a.x, mn, mx), clamp(a.y, mn, mx)); }


inline int max_index(const Vec2 &a) { return a.x > a.y ? 0 : 1; }
inline float max(const Vec2 &a) { return a[max_index(a)]; }
inline Vec2 cmp_wise_max(const Vec2 &a, const Vec2 &b) { return Vec2(max(a.x, b.x), max(a.y, b.y)); }


inline int min_index(const Vec2 &a) { return a.x < a.y ? 0 : 1; }
inline float min(const Vec2 &a) { return a[min_index(a)]; }
inline Vec2 cmp_wise_min(const Vec2 &a, const Vec2 &b) { return Vec2(min(a.x, b.x), min(a.y, b.y)); }

inline Vec2 round(const Vec2 &v) { return Vec2(round(v.x), round(v.y)); }

/********************* 3D Vector *********************/

union Vec3 {
  float V[3];
  struct { float x, y, z; };
  struct { float r, g, b; };

  Vec3()                          : x(0), y(0), z(0){};
  Vec3(float v)                   : x(v), y(v), z(v){};
  Vec3(float x, float y, float z) : x(x), y(y), z(z){};

  int num_dims() { return 3; }

  float &operator[] (int idx)       { return V[idx]; }
  float  operator[] (int idx) const { return V[idx]; };
};

inline std::ostream &operator<<(std::ostream &os, const Vec3 &t) {
  os << std::setprecision(3) << t.x << " " << t.y << " " << t.z << " ";
  return os;
}

inline Vec3 operator+(const Vec3 &a, const Vec3 &b) {  return Vec3(a.x + b.x, a.y + b.y, a.z + b.z); }  
inline Vec3 operator+(const Vec3 &v, const float t) {  return Vec3(v.x + t, v.y + t, v.z + t); }
inline Vec3 operator+(const float t, const Vec3 &v) {  return Vec3(v.x + t, v.y + t, v.z + t); }

inline Vec3 operator-(const Vec3 &a) {  return Vec3(-a.x, -a.y, -a.z); }
inline Vec3 operator-(const Vec3 &a, const Vec3 &b) { return Vec3(a.x - b.x, a.y - b.y, a.z - b.z); }
inline Vec3 operator-(const Vec3 &v, const float t) { return Vec3(v.x - t, v.y - t, v.z - t); }
inline Vec3 operator-(const float t, const Vec3 &v) { return Vec3(t - v.x, t - v.y, t - v.z); }

inline Vec3 operator*(const Vec3 &a, const Vec3 &b) { return Vec3(a.x * b.x, a.y * b.y, a.z * b.z); }
inline Vec3 operator*(const float t, const Vec3 &v) { return Vec3(t * v.x, t * v.y, t * v.z); }
inline Vec3 operator*(const Vec3 &v, const float t) { return Vec3(v.x * t, v.y * t, v.z * t); }

inline Vec3 operator/(const float t, const Vec3 &v) { return Vec3(t / v.x, t / v.y, t / v.z); }
inline Vec3 operator/(const Vec3 &v, const float t) { return Vec3(v.x / t, v.y / t, v.z / t); }

inline float dot(const Vec3 &a, const Vec3 &b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
inline float length(const Vec3 &a) { return sqrt(dot(a, a)); }
inline float length_sq(const Vec3 &a) { return dot(a, a); }
inline Vec3 normalized(const Vec3 &a) { return a / length(a); }
inline Vec3 mult(const Vec3 &a, const Vec3 &b) { return Vec3(a.x * b.x, a.y * b.y, a.z * b.z); }
inline Vec3 floor(const Vec3 &a) { return Vec3(floor(a.x), floor(a.y), floor(a.z)); }
inline Vec3 sqrt(const Vec3 &v) { return Vec3(sqrt(v.x), sqrt(v.y), sqrt(v.z)); }

inline Vec3 cross(const Vec3 &a, const Vec3 &b) { 
  return Vec3( a.y * b.z - a.z * b.y, 
              -a.x * b.z + a.z * b.x, 
               a.x * b.y - a.y * b.x); 
}

inline Vec3 &operator+=(Vec3 &a, const Vec3 &b) { return a = a + b; }
inline Vec3 &operator+=(Vec3 &a, const float &b) { return a = a + b; }

inline Vec3 &operator-=(Vec3 &a, const Vec3 &b) { return a = a - b; }
inline Vec3 &operator-=(Vec3 &a, const float b) { return a = a - b; }

inline Vec3 &operator*=(Vec3 &a, const Vec3 &b) { return a = a * b; }
inline Vec3 &operator*=(Vec3 &a, const float b) { return a = a * b; }

inline Vec3 &operator/=(Vec3 &a, const float b) { return a = a / b; }

inline Vec3 clamp01(const Vec3 &a) { return Vec3(clamp01(a.x), clamp01(a.y), clamp01(a.z)); }
inline Vec3 clamp(const Vec3 &a, float mn, float mx) { return Vec3(clamp(a.x, mn, mx), clamp(a.y, mn, mx), clamp(a.z, mn, mx)); }

inline int max_index(const Vec3 &a) { return a.x > a.y ? (a.x > a.z ? 0 : 2) : (a.y > a.z ? 1 : 2); }
inline float max(const Vec3 &a) { return a[max_index(a)]; }
inline Vec3 cmp_wise_max(const Vec3 &a, const Vec3 &b) { return Vec3(max(a.x, b.x), max(a.y, b.y), max(a.z, b.z)); }

inline int min_index(const Vec3 &a) { return a.x < a.y ? (a.x < a.z ? 0 : 2) : (a.y < a.z ? 1 : 2); }
inline float min(const Vec3 &a) { return a[min_index(a)]; }
inline Vec3 cmp_wise_min(const Vec3 &a, const Vec3 &b) { return Vec3(min(a.x, b.x), min(a.y, b.y), min(a.z, b.z)); }

inline Vec3 round(const Vec3 &v) { return Vec3(round(v.x), round(v.y), round(v.z)); }