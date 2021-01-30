#ifndef __IMPLICIT_H__
#define __IMPLICIT_H__

#include "core/object.h"

struct Implicit : Object {
  Implicit(Material *mat) : Object(mat){};
  bool hit(Ray &r, HitRec &rec);
  void finalize();
  Vec sample();

  virtual double F(double x, double y, double z) = 0;

  AABB localBouds;
};

struct TangleCube : Implicit {
  TangleCube(Material *mat, double offset = 0) : Implicit(mat), offset(offset) {
    bounds = AABB(3);
  };
  double F(double x, double y, double z) {
    return x * x * x * x - 5 * x * x + y * y * y * y - 5 * y * y +
           z * z * z * z - 5 * z * z + 11.8 + offset;
  }
  double offset;
};

struct Torus : Implicit {
  Torus(Material *mat) : Implicit(mat) { bounds = AABB(2); };
  double F(double x, double y, double z) {
    double R = 1;
    double a = 0.5;
    double r = x * x + y * y + z * z;
    double s = (r + R * R - a * a);
    return s * s - 4 * R * R * (x * x + y * y);
  }
};

struct Tetrahedral : Implicit {
  Tetrahedral(Material *mat, double offset = 0)
      : Implicit(mat), offset(offset) {
    bounds = AABB(4);
  };
  double F(double x, double y, double z) {
    return x * x * x * x + 2 * x * x * y * y + 2 * x * x * z * z +
           y * y * y * y + 2 * y * y * z * z + z * z * z * z + 8 * x * y * z -
           10 * x * x - 10 * y * y - 10 * z * z + 25 + offset;
  }
  double offset;
};

struct Chubbs : Implicit {
  Chubbs(Material *mat, double offset = 0) : Implicit(mat), offset(offset) {
    bounds = AABB(1.5);
  };
  double F(double x, double y, double z) {
    return x * x * x * x + y * y * y * y + z * z * z * z - x * x - y * y -
           z * z + 0.5 + offset;
  }
  double offset;
};

struct Sweet : Implicit {
  Sweet(Material *mat, double offset = 0) : Implicit(mat), offset(offset) {
    bounds = AABB(4);
  };
  double F(double x, double y, double z) {
    double k = (x * x) + ((2.25) * z * z) + (y * y) - 1;
    return k * k * k - ((x * x) * (y * y * y)) -
           ((.1125) * (z * z) * (y * y * y)) + offset;
  }
  double offset;
};

struct Blobs : Implicit {
  Blobs(Material *mat, double offset = 0) : Implicit(mat), offset(offset) {
    bounds = AABB(4);
  };
  double F(double x, double y, double z) {
    return x * x + y * y + z * z + sin(4 * x) + sin(4 * y) + sin(4 * z) - 1 +
           offset;
  }
  double offset;
};

struct Bifolia : Implicit {
  Bifolia(Material *mat, double offset = 0) : Implicit(mat), offset(offset) {
    bounds = AABB(2);
  };
  double F(double x, double y, double z) {
    return (x * x) * (y * y) * 2.0 + (x * x) * (z * z) * 2.0 +
           (y * y) * (z * z) * 2.0 - (x * x) * y * 3.0 - y * (z * z) * 3.0 +
           x * x * x * x + y * y * y * y + z * z * z * z + offset;
  }
  double offset;
};

struct BohemianDome : Implicit {
  BohemianDome(Material *mat, double offset = 0)
      : Implicit(mat), offset(offset) {
    bounds = AABB(2);
  };
  double F(double x, double y, double z) {
    return -((x * x) * (y * y) * -2.0 + (x * x) * (z * z) * 2.0 -
             (y * y) * (z * z) * 2.0 - x * x * x * x + (y * y) * 4.0 -
             y * y * y * y - z * z * z * z + offset);
  }
  double offset;
};

struct Gumdrop : Implicit {
  Gumdrop(Material *mat, double offset = 0) : Implicit(mat), offset(offset) {
    bounds = AABB(2.3);
  };
  double F(double x, double y, double z) {
    return ((x * x) * (y * y + z * z) * 1.7E+1 + pow(y * y + z * z, 2.0) * 4.0 -
            (x * x) * 2.0E+1 + (x * x * x * x) * 4.0 - (y * y) * 2.0E+1 -
            (z * z) * 2.0E+1 + 1.7E+1 + offset);
  }
  double offset;
};

struct WiffleCube : Implicit {
  WiffleCube(Material *mat, double offset = 0)
      : Implicit(mat) {
    bounds = AABB(2.3);
    b = lerp(offset, 0.38, 0.58);
  };
  double F(double x, double y, double z) {
    return -(-1.0 / pow(a, 1.2E+1) * 1.0 / pow(x * x + y * y + z * z, 6.0) -
             pow(b, 4.8E+1) * pow(x * x * x * x * x * x * x * x +
                                      y * y * y * y * y * y * y * y +
                                      z * z * z * z * z * z * z * z,
                                  6.0) +
             1.0 + offset);
  }
  double a = 1.0 / 2.3;
  double b = 0.5;
  double offset;
};

struct Orthocircle : Implicit {
  Orthocircle(Material *mat, double off = 0) : Implicit(mat) {
    a = off * 0.1 + 0.04;
    b = off;
    offset = -off * .01;
    bounds = AABB(1.5);
  };
  double F(double x, double y, double z) {
    return -(a * a) * (b * (x * x + y * y + z * z) + 1.0) +
           (pow(x * x + y * y - 1.0, 2.0) + z * z) *
               (pow(x * x + z * z - 1.0, 2.0) + y * y) *
               (pow(y * y + z * z - 1.0, 2.0) + x * x) +
           offset;
  }
  double a = 0.075;
  double b = 0.3;
  double offset;
};

#endif // __IMPLICIT_H__
