#pragma once

#include <core/object.h>

namespace SDF {
  inline float Fmod(float a, float p) {
    if (a > 0) return  fmod(a, p);
    else       return -fmod(a,p);
  }

  inline Vec3 Fmod(const Vec3& a, float p) {
    return Vec3(
      Fmod(a.x, p),
      Fmod(a.y, p),
      Fmod(a.z, p)
    );
  }

  inline Vec3 Fmod(const Vec3& a, const Vec3& p) {
    return Vec3(
      Fmod(a.x, p.x),
      Fmod(a.y, p.y),
      Fmod(a.z, p.z)
    );
  }

  // Fold a point across a plane defined by a point and a normal
  // The normal should face the side to be reflected
  inline Vec3 fold(const Vec3& p, const Vec3& plane_p, const Vec3& plane_n) {
    // Center plane on origin for distance calculation
    float distToPlane = dot(p - plane_p, plane_n);
    
    // We only want to reflect if the dist is negative
    distToPlane = min(distToPlane, 0.0);
    return p - 2.0 * distToPlane * plane_n;
  }

  inline float tetrahedron(const Vec3& p) {
    return (max(
      abs(p.x + p.y) - p.z,
      abs(p.x - p.y) + p.z
    ) - 1.0) / sqrt(3.);
  }

  inline float sphere(const Vec3& p, float radius=1.0) {
    return length(p) - radius;
  }

  inline Vec3 repeat(const Vec3& p, const Vec3& dims) {
    return Fmod(p, dims) - dims * 0.5;
  }

  inline Vec3 repeatX(const Vec3& p, float off) {
    return Vec3(
      Fmod(p.x, off) - off * 0.5,
      p.y,
      p.z
    );
  }

  inline Vec3 repeatY(const Vec3& p, float off) {
    return Vec3(
      p.x,
      Fmod(p.y, off) - off * 0.5,
      p.z
    );
  }

  inline Vec3 repeatZ(const Vec3& p, float off) {
    return Vec3(
      p.x,
      p.y,
      Fmod(p.z, off) - off * 0.5
    );
  }
};

struct SDFObject : Object {
  SDFObject(BSDF *mat) : Object(mat){};
  bool hit(Ray &r, HitRec &rec);

  virtual float F(const Vec3& p) = 0;
};

struct SphereSDF : SDFObject {
  using SDFObject::SDFObject;
  float F(const Vec3& p) {
    return length(p) - 1;
  }
};

struct InfiniteSphereSDF : SDFObject {
  using SDFObject::SDFObject;
  float F(const Vec3& point) {
    Vec3 p = point;
    if (p.z < minZ) p.z = minZ;
    p = SDF::repeatX(p, 2);
    p = SDF::repeatY(p, 2);
    p = SDF::repeatZ(p, 2);
    return SDF::sphere(p, 0.4);
  }
  float minZ = -12;
};

struct MandelBulbSDF : SDFObject {
  MandelBulbSDF(BSDF* mat, float power=8) : SDFObject(mat), power(power) {};
  float F(const Vec3& p) {
    // initialisation
    Vec3 z = p;
    float dr = 1; // running derivative
    float r = 0;  // escape time length

    for (int i = 0; i < iterations; i++) {
      r = length(z);
      if (r > depth_of_field) break;
      
      // conversion to polar coordinates
      float theta = acos(z.z / r);
      float phi = atan2(z.y, z.x);
      dr = pow(r, power - 1.0) * power * dr + 1.0;
      
      // scaling and rotation
      float zr = pow(r, power);
      theta *= power;
      phi *= power;

      // conversion back to cartesian coordinates
      z = Vec3(cos(phi) * sin(theta),
              sin(phi) * sin(theta),
               cos(theta));
      z = z * zr + p;
    }
    // distance estimator
    return 0.5 * log(r) * r / dr;
  }
  float power = 7;
  int iterations = 50;
  float depth_of_field = 20.0;
};

struct SerpinksiSDF : SDFObject {
  using SDFObject::SDFObject;
  float F(const Vec3& p) {
    Vec3 z = p;

    float scale = 1.0;
    for (int i = 0; i < iterations; i++) {
    	// Scale point toward corner vertex, update scale accumulator
      z = (z - verts[0]) * 2 + verts[0];
      scale *= 2.0;
      
      // Fold point across each plane
      for (int i = 1; i <= 3; i++) {
        // The plane is defined by:
        // Point on plane: The vertex that we are reflecting across
        // Plane normal: The direction from said vertex to the corner vertex
        Vec3 normal = normalized(verts[0] - verts[i]);
        z = SDF::fold(z, verts[i], normal);
      }
    }
    // Now that the space has been distorted by the IFS,
    // just return the distance to a tetrahedron
    // Divide by scale accumulator to correct the distance field
    return SDF::tetrahedron(z) / scale;
  }
  int iterations = 11;
  inline static Vec3 verts[4] = {Vec3(1.0, 1.0, 1.0),
                                 Vec3(-1.0, 1.0, -1.0),
                                 Vec3(-1.0, -1.0, 1.0),
                                 Vec3(1.0, -1.0, -1.0) };
};

typedef float (*SDFFunc)(const Vec3&);
struct CustomSDF : SDFObject {
  CustomSDF(SDFFunc func, BSDF* mat): SDFObject(mat), func(func) {};
  float F(const Vec3& p) { return func(p); }
  SDFFunc func;
};

