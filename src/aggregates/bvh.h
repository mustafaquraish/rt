#pragma once

#include <core/aabb.h>
#include <core/primitive.h>

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
  inline static bool time_build = true;
  inline static void set_time_build(bool state) { BVH::time_build = state; }

  enum BuildMethod {
    SAH,
    Median,
  };

  BVH(std::vector<Primitive *>&, BuildMethod=Median);
  ~BVH() {};
  bool hit(Ray& ray, HitRec& rec);
  
  /* Internal building utilities */
  BVHTree *build(std::vector<Primitive *>& list, int start, int end);
  int flatten(BVHTree *root);
  
  std::vector<BVHLinear> m_nodes;
  BuildMethod m_method;
};

