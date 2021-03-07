#ifndef __BVH_H__
#define __BVH_H__

#include "core/primitive.h"
#include "core/aabb.h"
#include <vector>

// Structure used when creating the BVH
struct BVHTree {
  AABB bounds;
  int primOff;    // Primitives offset
  int numPrims;   // 0 -> internal node
  BVHTree *a, *b;
};

// Structure used when storing the BVH
struct BVHLinear {
  AABB bounds;
  union {
    int primOff;    // leaf
    int child2Off;  // internal
  };
  uint16_t numPrims;  // 0 -> internal node
};

struct BVH : Aggregate {
  BVH(std::vector<Primitive *>& list);
  ~BVH() {};
  bool hit(Ray& ray, HitRec& rec);
  
  /* Internal building utilities */
  BVHTree *buildBVH(std::vector<Primitive *>& list, int start, int end);
  int flatten(BVHTree *root);
  
  std::vector<BVHLinear> nodes;
  std::vector<Primitive *> primitives;
};

#endif // __BVH_H__
