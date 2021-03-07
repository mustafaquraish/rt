#ifndef __BVH_H__
#define __BVH_H__

#include "core/primitive.h"
#include "core/aabb.h"
#include <vector>

struct BVHTree {
  AABB bounds;
  bool isLeaf = 0;
  int axis;
  int offset, numPrims;
  BVHTree *bva, *bvb;
  bool hit(Ray& ray, HitRec& rec, 
          std::vector<Primitive *>& list);
};

struct BVHLinear {
  AABB bounds;
  union {
    int primOff;   // leaf
    int child2Off;  // interior
  };
  uint16_t nPrimitives;  // 0 -> interior node
  uint8_t axis;          // interior node: xyz
  uint8_t pad[1];        // ensure 32 byte total size
};

struct BVH : Aggregate {
  BVH(std::vector<Primitive *>& list, int start=0, int end=-1);
  ~BVH() {};
  BVHTree *buildBVH(std::vector<Primitive *>& list, int start, int end);
  int flatten(BVHTree *root);
  bool hit(Ray& ray, HitRec& rec);
  
  BVHTree *bvh;
  std::vector<BVHLinear> nodes;
  std::vector<Primitive *> primitives;
};

#endif // __BVH_H__
