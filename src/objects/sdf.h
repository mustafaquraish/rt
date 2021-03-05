#ifndef __SDF_H__
#define __SDF_H__

#include "core/object.h"

namespace SDF {
  inline double Fmod(double a, double p) {
    if (a > 0) return  fmod(a, p);
    else       return -fmod(a,p);
  }

  inline Vec Fmod(const Vec& a, double p) {
    return Vec(
      Fmod(a.x, p),
      Fmod(a.y, p),
      Fmod(a.z, p)
    );
  }

  inline Vec Fmod(const Vec& a, const Vec& p) {
    return Vec(
      Fmod(a.x, p.x),
      Fmod(a.y, p.y),
      Fmod(a.z, p.z)
    );
  }

  inline Vec repeat(const Vec& p, const Vec& dims) {
    return Fmod(p, dims) - dims/2.0;
  }
};

struct SDFObject : Object {
  SDFObject(BSDF *mat) : Object(mat){};
  bool hit(Ray &r, HitRec &rec);

  virtual double F(const Vec& p) = 0;
};

struct SphereSDF : SDFObject {
  using SDFObject::SDFObject;
  double F(const Vec& p) {
    return length(p) - 1;
  }
};

struct InfiniteSphereSDF : SDFObject {
  using SDFObject::SDFObject;
  double F(const Vec& p) {
    Vec pp = p;
    if (pp.z < minZ) pp.z = minZ;
    return length( SDF::repeat(pp, Vec(2)) ) - 0.4;
  }
  double minZ = -12;
};

struct MandelBulbSDF : SDFObject {
  using SDFObject::SDFObject;
  double F(const Vec& p) {
    // initialisation
    Vec z = p;
    double dr = 1; // running derivative
    double r = 0; // escape time length
    // iteration
    int i;
    for (i = 0; i < iterations; i++) {
      r = length(z);
      if (r > depth_of_filed) {
        break;
      }
      // conversion to polar coordinates
      double theta = acos(z.z / r);
      double phi = atan2(z.y, z.x);
      dr = pow(r, power - 1.0) * power * dr + 1.0;
      // scaling and rotation
      double zr = pow(r, power);
      theta = theta * power;
      phi = phi * power;
      // conversion back to cartesian coordinates
      z = Vec(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));
      z = z * zr + p;
    }
    // distance estimator
    return 0.5 * log(r) * r / dr;
  }
  double power = 7;
  int iterations = 200;
  double depth_of_filed = 2.0;
};

typedef double (*SDFFunc)(const Vec&);
struct CustomSDF : SDFObject {
  CustomSDF(SDFFunc func, BSDF* mat): SDFObject(mat), func(func) {};
  double F(const Vec& p) { return func(p); }
  SDFFunc func;
};

#endif // __SDF_H__
