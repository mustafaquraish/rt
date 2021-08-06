#pragma once

#include <core/math.h>
#include <core/primitive.h>
#include <core/rng.h>
#include <core/texture.h>

struct BSDF {
  BSDF() {}
  BSDF(Colour col) : m_col(col) {}
  BSDF(Texture *tx) : m_tx(tx) {}
  
  virtual ~BSDF() { delete m_tx; };

  virtual Colour eval(HitRec& rec) = 0;
  virtual Colour sample(HitRec& rec, RNG& rng) = 0;
  virtual float pdf(HitRec& rec) = 0;
  virtual Colour emittance(HitRec& rec) { return Vec3(0); }

  bool isDiffuse()  const { return diffuse; }
  bool isEmitter()  const { return emitter; }
  bool isSpecular() const { return specular; }

  Colour col(HitRec& rec) { 
    if (m_tx != NULL) return m_tx->get(rec);
    return m_col; 
  }

  bool specular = false;
  bool diffuse = false;
  bool emitter = false;
  Colour m_col = Vec3(0, 1, 0);
  Texture *m_tx = NULL;
};

inline Vec3 toLocalFrame(const Vec3& v, const Vec3& n) {
  Vec3 s, t;
  if (abs(n.x) > abs(n.y)) {
    float invLen = 1.0f / sqrt(n.x * n.x + n.z * n.z);
    t = Vec3(n.z * invLen, 0.0f, -n.x * invLen);
  } else {
    float invLen = 1.0f / sqrt(n.y * n.y + n.z * n.z);
    t = Vec3(0.0f, n.z * invLen, -n.y * invLen);
  }
  s = cross(t, n);

  return Vec3(dot(v, s), dot(v, t), dot(v, n));
}

inline Vec3 toWorld(const Vec3& v, const Vec3& n) {
  Vec3 s, t;
  if (abs(n.x) > abs(n.y)) {
    float invLen = 1.0f / sqrt(n.x * n.x + n.z * n.z);
    t = Vec3(n.z * invLen, 0.0f, -n.x * invLen);
  } else {
    float invLen = 1.0f / sqrt(n.y * n.y + n.z * n.z);
    t = Vec3(0.0f, n.z * invLen, -n.y * invLen);
  }
  s = cross(t, n);

  return s * v.x + t * v.y + n * v.z;
}

inline float cosTheta(const Vec3 &v)    { return v.z;                         }
inline float cos2Theta(const Vec3 &v)   { return v.z * v.z;                   }
inline float absCosTheta(const Vec3 &v) { return v.z < 0.f ? -v.z : v.z;      }
inline float sin2Theta(const Vec3 &v)   { return 1.f - cos2Theta(v);          }
inline float sinTheta(const Vec3 &v)    { return sqrtf(sin2Theta(v));         }
inline float tanTheta(const Vec3 &v)    { return sinTheta(v) / cosTheta(v);  }
inline float tan2Theta(const Vec3 &v)   { return sin2Theta(v) / cos2Theta(v); }

inline float cosPhi(const Vec3 &v) {
  float sintheta = sinTheta(v);
  float x = (sintheta == 0.f) ? 0.f : v.x / sintheta;
  return (x < -1.f ? -1.f : (x > 1.f ? 1.f : x));
}

inline float sinPhi(const Vec3 &v) {
  float sintheta = sinTheta(v);
  float x = (sintheta == 0.f) ? 0.f : v.y / sintheta;
  return (x < -1.f ? -1.f : (x > 1.f ? 1.f : x));
}

inline float cos2Phi(const Vec3 &v) { float x = cosPhi(v); return x * x; }
inline float sin2Phi(const Vec3 &v) { float x = sinPhi(v); return x * x; }

