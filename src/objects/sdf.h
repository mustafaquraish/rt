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

  // Fold a point across a plane defined by a point and a normal
  // The normal should face the side to be reflected
  inline Vec fold(const Vec& p, const Vec& plane_p, const Vec& plane_n) {
    // Center plane on origin for distance calculation
    double distToPlane = dot(p - plane_p, plane_n);
    
    // We only want to reflect if the dist is negative
    distToPlane = min(distToPlane, 0.0);
    return p - 2.0 * distToPlane * plane_n;
  }

  inline double tetrahedron(const Vec& p) {
    return (max(
      abs(p.x + p.y) - p.z,
      abs(p.x - p.y) + p.z
    ) - 1.0) / sqrt(3.);
  }

  inline double sphere(const Vec& p, double radius=1.0) {
    return length(p) - radius;
  }

  inline Vec repeat(const Vec& p, const Vec& dims) {
    return Fmod(p, dims) - dims * 0.5;
  }

  inline Vec repeatX(const Vec& p, double off) {
    return Vec(
      Fmod(p.x, off) - off * 0.5,
      p.y,
      p.z
    );
  }

  inline Vec repeatY(const Vec& p, double off) {
    return Vec(
      p.x,
      Fmod(p.y, off) - off * 0.5,
      p.z
    );
  }

  inline Vec repeatZ(const Vec& p, double off) {
    return Vec(
      p.x,
      p.y,
      Fmod(p.z, off) - off * 0.5
    );
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
    pp = SDF::repeatX(pp, 2);
    pp = SDF::repeatY(pp, 2);
    pp = SDF::repeatZ(pp, 2);
    return SDF::sphere(pp, 0.4);
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
      if (r > depth_of_field) {
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
  int iterations = 50;
  double depth_of_field = 20.0;
};

struct SerpinksiSDF : SDFObject {
  using SDFObject::SDFObject;
  double F(const Vec& p) {
    Vec z = p;

    double scale = 1.0;
    for (int i = 0; i < iterations; i++) {
    	// Scale point toward corner vertex, update scale accumulator
      z -= vertices[0];
      z *= 2.0;
      z += vertices[0];
      
      scale *= 2.0;
      
      // Fold point across each plane
      for (int i = 1; i <= 3; i++) {
        // The plane is defined by:
        // Point on plane: The vertex that we are reflecting across
        // Plane normal: The direction from said vertex to the corner vertex
        Vec normal = norm(vertices[0] - vertices[i]); 
        z = SDF::fold(z, vertices[i], normal);
      }
    }
    // Now that the space has been distorted by the IFS,
    // just return the distance to a tetrahedron
    // Divide by scale accumulator to correct the distance field
    return SDF::tetrahedron(z) / scale;
  }
  int iterations = 11;
  inline static Vec vertices[4] = { Vec(1.0, 1.0, 1.0),
                                    Vec(-1.0, 1.0, -1.0),
                                    Vec(-1.0, -1.0, 1.0),
                                    Vec(1.0, -1.0, -1.0) };
};

typedef double (*SDFFunc)(const Vec&);
struct CustomSDF : SDFObject {
  CustomSDF(SDFFunc func, BSDF* mat): SDFObject(mat), func(func) {};
  double F(const Vec& p) { return func(p); }
  SDFFunc func;
};

#endif // __SDF_H__
