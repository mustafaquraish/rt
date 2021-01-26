/**
 * This is NOT an object, but it just extends Primitive. Used in the 
 * TriangleMesh() object.
 */

#ifndef __TRIANGLE_H__
#define __TRIANGLE_H__

#include "core/primitive.h"

struct Triangle : Primitive {
  Triangle(Vec  p0, Vec  p1, Vec  p2, 
           Vec  n0, Vec  n1, Vec  n2, 
           Vec ab0, Vec ab1, Vec ab2);
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
