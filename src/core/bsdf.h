#ifndef __BSDF_H__
#define __BSDF_H__

#include "core/math.h"
#include "core/primitive.h"
#include "core/rng.h"

struct BSDFRec {
  BSDFRec(const Vec& d, const HitRec& rec, RNG& rng) : 
    wo(-d), n(rec.n), u(rec.u), v(rec.v), p(rec.p), rng(rng) {}
  Vec wo;      // Outgoing direction
  Vec wi;      // Incident direction (to be sampled)
  Vec n;       // Normal at the shading point
  Vec p;       // Shading point
  double u, v; // Texture coordinates for intersection point
  RNG& rng;
};

struct BSDF {
  BSDF(Colour col) : col(col) {}

  virtual Colour eval(BSDFRec& bRec) = 0;
  virtual Colour sample(BSDFRec& bRec) = 0;
  virtual double pdf(BSDFRec& bRec) = 0;
  virtual Colour emittance(BSDFRec& bRec) { return Vec(0); }

  bool isDiffuse() const { return diffuse; }
  bool isEmitter() const { return emitter; }
  bool isSpecular() const { return specular; }

  bool specular = false;
  bool diffuse = false;
  bool emitter = false;
  Colour col;

  Vec toLocalFrame(const Vec& v, const Vec& n);
  Vec toWorld(const Vec& v, const Vec& n);

  double sinTheta(const Vec& v);
  double sinPhi(const Vec& v);
  double sin2Theta(const Vec& v);
  double sin2Phi(const Vec& v);

  double cosTheta(const Vec& v);
  double cosPhi(const Vec& v);
  double cos2Theta(const Vec& v);
  double cos2Phi(const Vec& v);
  double absCosTheta(const Vec& v);

  double tanTheta(const Vec& v);
  double tanPhi(const Vec& v);
  double tan2Theta(const Vec& v);
  double tan2Phi(const Vec& v);
};

inline Vec BSDF::toLocalFrame(const Vec& v, const Vec& n) {
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

inline Vec BSDF::toWorld(const Vec& v, const Vec& n) {
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


inline double BSDF::cosTheta(const Vec &v)    { return v.z;                         }
inline double BSDF::cos2Theta(const Vec &v)   { return v.z * v.z;                   }
inline double BSDF::absCosTheta(const Vec &v) { return v.z < 0.f ? -v.z : v.z;      }
inline double BSDF::sin2Theta(const Vec &v)   { return 1.f - cos2Theta(v);          }
inline double BSDF::sinTheta(const Vec &v)    { return sqrtf(sin2Theta(v));         }
inline double BSDF::tanTheta(const Vec &v)    { return sinTheta(v)  / cosTheta(v);  }
inline double BSDF::tan2Theta(const Vec &v)   { return sin2Theta(v) / cos2Theta(v); }

inline double BSDF::cosPhi(const Vec &v) {
  double sintheta = sinTheta(v);
  double x = (sintheta == 0.f) ? 0.f : v.x / sintheta;
  return (x < -1.f ? -1.f : (x > 1.f ? 1.f : x));
}

inline double BSDF::sinPhi(const Vec &v) {
  double sintheta = sinTheta(v);
  double x = (sintheta == 0.f) ? 0.f : v.y / sintheta;
  return (x < -1.f ? -1.f : (x > 1.f ? 1.f : x));
}

inline double BSDF::cos2Phi(const Vec &v) { double x = cosPhi(v); return x * x; }
inline double BSDF::sin2Phi(const Vec &v) { double x = sinPhi(v); return x * x; }

#endif // __BSDF_H__
