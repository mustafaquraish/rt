#ifndef __IMPLICIT_H__
#define __IMPLICIT_H__

#include "core/object.h"

struct Implicit : Object {
  Implicit(Material *mat) : Object(mat) {};
  bool hit(Ray& r, HitRec &rec);
  void finalize();
  Vec sample();

  virtual double F(double x, double y, double z) = 0;

  AABB localBouds;
};

struct TangleCube : Implicit {
  TangleCube(Material *mat) : Implicit(mat) { bounds = AABB(3); };
  double F(double x, double y, double z) {
    return x*x*x*x - 5*x*x + y*y*y*y - 5*y*y + z*z*z*z - 5*z*z + 11.8;
  }
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
  Tetrahedral(Material *mat) : Implicit(mat) { bounds = AABB(4); };
  double F(double x, double y, double z) {
    return x*x*x*x + 2 * x*x * y*y + 2 * x*x * z*z +
           y*y*y*y + 2 * y*y * z*z + 
           z*z*z*z + 8 * x*y*z - 10 * x*x - 10*y*y - 10*z*z + 25;
  }
};

struct Chubbs : Implicit {
  Chubbs(Material *mat) : Implicit(mat) { bounds = AABB(1); };
  double F(double x, double y, double z) {
    return x*x*x*x + y*y*y*y + z*z*z*z - x*x - y*y - z*z + 0.5;
  }
};

struct HummingBird : Implicit {
  HummingBird(Material *mat) : Implicit(mat) { bounds = AABB(4); };
  double F(double x, double y, double z) {
    return (x*x*x)+(y*y*z*z)-(z*z);
  }
};

struct Sweet : Implicit {
  Sweet(Material *mat) : Implicit(mat) { bounds = AABB(4); };
  double F(double x, double y, double z) {
    double k = (x*x)+((2.25)*z*z)+(y*y)-1;
    return k*k*k-((x*x)*(y*y*y))-((.1125)*(z*z)*(y*y*y));
  }
};

#endif // __IMPLICIT_H__
