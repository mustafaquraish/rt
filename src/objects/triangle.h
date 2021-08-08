/**
 * This is NOT an object, but it just extends Primitive. Used in the 
 * TriangleMesh() object.
 */

#pragma once

#include <core/primitive.h>

struct Triangle : Primitive {
  Triangle(Vec3  p0, Vec3  p1, Vec3  p2,
           Vec3  n0, Vec3  n1, Vec3  n2,
           Vec3 ab0, Vec3 ab1, Vec3 ab2);

  Triangle(Vec3  p0, Vec3  p1, Vec3  p2,
           Vec3  n0, Vec3  n1, Vec3  n2);
           
  Triangle(Vec3  p0, Vec3  p1, Vec3  p2);
  
  bool hit(Ray& r, HitRec &rec);
  Vec3 baryCentricWeights(const Vec3& p);

  Vec3 p[3];
  Vec3 n[3];
  Vec3 uv[3];
};

