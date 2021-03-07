#ifndef __BSDF_H__
#define __BSDF_H__

#include "core/math.h"
#include "core/primitive.h"
#include "core/rng.h"
#include "core/texture.h"

struct BSDF {
  BSDF() {}
  BSDF(Colour col) : m_col(col) {}
  BSDF(Texture *tx) : m_tx(tx) {}
  
  virtual ~BSDF() { delete m_tx; };

  virtual Colour eval(HitRec& rec) = 0;
  virtual Colour sample(HitRec& rec, RNG& rng) = 0;
  virtual double pdf(HitRec& rec) = 0;
  virtual Colour emittance(HitRec& rec) { return Vec(0); }

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
  Colour m_col = Vec(0, 1, 0);
  Texture *m_tx = NULL;
};

inline Vec toLocalFrame(const Vec& v, const Vec& n) {
  Vec s, t;
  if (abs(n.x) > abs(n.y)) {
    double invLen = 1.0f / sqrt(n.x * n.x + n.z * n.z);
    t = Vec(n.z * invLen, 0.0f, -n.x * invLen);
  } else {
    double invLen = 1.0f / sqrt(n.y * n.y + n.z * n.z);
    t = Vec(0.0f, n.z * invLen, -n.y * invLen);
  }
  s = cross(t, n);

  return Vec(dot(v, s), dot(v, t), dot(v, n));
}

inline Vec toWorld(const Vec& v, const Vec& n) {
  Vec s, t;
  if (abs(n.x) > abs(n.y)) {
    double invLen = 1.0f / sqrt(n.x * n.x + n.z * n.z);
    t = Vec(n.z * invLen, 0.0f, -n.x * invLen);
  } else {
    double invLen = 1.0f / sqrt(n.y * n.y + n.z * n.z);
    t = Vec(0.0f, n.z * invLen, -n.y * invLen);
  }
  s = cross(t, n);

  return s * v.x + t * v.y + n * v.z;
}

inline double cosTheta(const Vec &v)    { return v.z;                         }
inline double cos2Theta(const Vec &v)   { return v.z * v.z;                   }
inline double absCosTheta(const Vec &v) { return v.z < 0.f ? -v.z : v.z;      }
inline double sin2Theta(const Vec &v)   { return 1.f - cos2Theta(v);          }
inline double sinTheta(const Vec &v)    { return sqrtf(sin2Theta(v));         }
inline double tanTheta(const Vec &v)    { return sinTheta(v)  / cosTheta(v);  }
inline double tan2Theta(const Vec &v)   { return sin2Theta(v) / cos2Theta(v); }

inline double cosPhi(const Vec &v) {
  double sintheta = sinTheta(v);
  double x = (sintheta == 0.f) ? 0.f : v.x / sintheta;
  return (x < -1.f ? -1.f : (x > 1.f ? 1.f : x));
}

inline double sinPhi(const Vec &v) {
  double sintheta = sinTheta(v);
  double x = (sintheta == 0.f) ? 0.f : v.y / sintheta;
  return (x < -1.f ? -1.f : (x > 1.f ? 1.f : x));
}

inline double cos2Phi(const Vec &v) { double x = cosPhi(v); return x * x; }
inline double sin2Phi(const Vec &v) { double x = sinPhi(v); return x * x; }

#endif // __BSDF_H__
