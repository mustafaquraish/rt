#pragma once

#include <core/object.h>

struct Implicit : Object {
  Implicit(BSDF *mat) : Object(mat){};
  bool hit(Ray &r, HitRec &rec);
  void finalize();
  Vec3 sample(float *pdf, RNG& rng);

  virtual float F(float x, float y, float z) = 0;

  AABB localBouds;
};

struct TangleCube : Implicit {
  TangleCube(BSDF *mat, float offset = 0) : Implicit(mat), offset(offset) {
    bounds = AABB(3);
  };
  float F(float x, float y, float z) {
    return x * x * x * x - 5 * x * x + y * y * y * y - 5 * y * y +
           z * z * z * z - 5 * z * z + 11.8 + offset;
  }
  float offset;
};

struct Torus : Implicit {
  Torus(BSDF *mat) : Implicit(mat) { bounds = AABB(2); };
  float F(float x, float y, float z) {
    float R = 1;
    float a = 0.5;
    float r = x * x + y * y + z * z;
    float s = (r + R * R - a * a);
    return s * s - 4 * R * R * (x * x + y * y);
  }
};

struct Tetrahedral : Implicit {
  Tetrahedral(BSDF *mat, float offset = 0)
      : Implicit(mat), offset(offset) {
    bounds = AABB(4);
  };
  float F(float x, float y, float z) {
    return x * x * x * x + 2 * x * x * y * y + 2 * x * x * z * z +
           y * y * y * y + 2 * y * y * z * z + z * z * z * z + 8 * x * y * z -
           10 * x * x - 10 * y * y - 10 * z * z + 25 + offset;
  }
  float offset;
};

struct Chubbs : Implicit {
  Chubbs(BSDF *mat, float offset = 0) : Implicit(mat), offset(offset) {
    bounds = AABB(1.5);
  };
  float F(float x, float y, float z) {
    return x * x * x * x + y * y * y * y + z * z * z * z - x * x - y * y -
           z * z + 0.5 + offset;
  }
  float offset;
};

struct Sweet : Implicit {
  Sweet(BSDF *mat, float offset = 0) : Implicit(mat), offset(offset) {
    bounds = AABB(4);
  };
  float F(float x, float y, float z) {
    float k = (x * x) + ((2.25) * z * z) + (y * y) - 1;
    return k * k * k - ((x * x) * (y * y * y)) -
           ((.1125) * (z * z) * (y * y * y)) + offset;
  }
  float offset;
};

struct Blobs : Implicit {
  Blobs(BSDF *mat, float offset = 0) : Implicit(mat), offset(offset) {
    bounds = AABB(4);
  };
  float F(float x, float y, float z) {
    return x * x + y * y + z * z + sin(4 * x) + sin(4 * y) + sin(4 * z) - 1 +
           offset;
  }
  float offset;
};

struct Bifolia : Implicit {
  Bifolia(BSDF *mat, float offset = 0) : Implicit(mat), offset(offset) {
    bounds = AABB(2);
  };
  float F(float x, float y, float z) {
    return (x * x) * (y * y) * 2.0 + (x * x) * (z * z) * 2.0 +
           (y * y) * (z * z) * 2.0 - (x * x) * y * 3.0 - y * (z * z) * 3.0 +
           x * x * x * x + y * y * y * y + z * z * z * z + offset;
  }
  float offset;
};

struct BohemianDome : Implicit {
  BohemianDome(BSDF *mat, float offset = 0)
      : Implicit(mat), offset(offset) {
    bounds = AABB(2);
  };
  float F(float x, float y, float z) {
    return -((x * x) * (y * y) * -2.0 + (x * x) * (z * z) * 2.0 -
             (y * y) * (z * z) * 2.0 - x * x * x * x + (y * y) * 4.0 -
             y * y * y * y - z * z * z * z + offset);
  }
  float offset;
};

struct Gumdrop : Implicit {
  Gumdrop(BSDF *mat, float offset = 0) : Implicit(mat), offset(offset) {
    bounds = AABB(2.3);
  };
  float F(float x, float y, float z) {
    return ((x * x) * (y * y + z * z) * 1.7E+1 + pow(y * y + z * z, 2.0) * 4.0 -
            (x * x) * 2.0E+1 + (x * x * x * x) * 4.0 - (y * y) * 2.0E+1 -
            (z * z) * 2.0E+1 + 1.7E+1 + offset);
  }
  float offset;
};

struct WiffleCube : Implicit {
  WiffleCube(BSDF *mat, float offset = 0)
      : Implicit(mat) {
    bounds = AABB(2.3);
    b = lerp(offset, 0.38, 0.58);
  };
  float F(float x, float y, float z) {
    return -(-1.0 / pow(a, 1.2E+1) * 1.0 / pow(x * x + y * y + z * z, 6.0) -
             pow(b, 4.8E+1) * pow(x * x * x * x * x * x * x * x +
                                      y * y * y * y * y * y * y * y +
                                      z * z * z * z * z * z * z * z,
                                  6.0) +
             1.0 + offset);
  }
  float a = 1.0 / 2.3;
  float b = 0.5;
  float offset;
};

struct Orthocircle : Implicit {
  Orthocircle(BSDF *mat, float off = 0) : Implicit(mat) {
    a = off * 0.1 + 0.04;
    b = off;
    offset = -off * .01;
    bounds = AABB(1.5);
  };
  float F(float x, float y, float z) {
    return -(a * a) * (b * (x * x + y * y + z * z) + 1.0) +
           (pow(x * x + y * y - 1.0, 2.0) + z * z) *
               (pow(x * x + z * z - 1.0, 2.0) + y * y) *
               (pow(y * y + z * z - 1.0, 2.0) + x * x) +
           offset;
  }
  float a = 0.075;
  float b = 0.3;
  float offset;
};

