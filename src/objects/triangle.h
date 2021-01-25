/**
 * This is NOT an object, but it just extends Primitive. Used in the 
 * TriangleMesh() object.
 */

#ifndef __TRIANGLE_H__
#define __TRIANGLE_H__

#include "core/primitive.h"

struct Triangle : Primitive {
  Triangle(Vec p0, Vec p1, Vec p2,
           Vec n0, Vec n1, Vec n2,
           Vec ab0, Vec ab1, Vec ab2) {
    p[0] = p0; n[0] = n0; ab[0] = ab0;
    p[1] = p1; n[1] = n1; ab[1] = ab1;
    p[2] = p2; n[2] = n2; ab[2] = ab2;
    bounds = AABB(0);
    bounds.addPoint(p0);
    bounds.addPoint(p1);
    bounds.addPoint(p2);
    // std::cout << "Triangle bounds: " << bounds << std::endl;
    // std::cout << "Triangle bounds: " << getBounds() << std::endl;
  };
  bool hit(Ray& r, HitRec &rec);
  // Get primitive bounds
  AABB getBounds() const { return bounds; };
  Vec baryCentricWeights(const Vec& p);

  Vec p[3];
  Vec n[3];
  Vec ab[3];
  AABB bounds;
};

#endif // __TRIANGLE_H__
