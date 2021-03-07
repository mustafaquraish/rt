#ifndef __KDTREE_H__
#define __KDTREE_H__

#include "core/primitive.h"
#include "core/aabb.h"
#include <vector>

// Structure used when creating the BVH
struct KDTreeNode {
  int primOff;    // Primitives offset
  int numPrims;   // 0 -> internal node
  int axis;
  double splitPos;
  KDTreeNode *a, *b;
};

struct KDTree : Aggregate {
  KDTree(std::vector<Primitive *>& list);
  ~KDTree() {};
  bool hit(Ray& ray, HitRec& rec);
  
  KDTreeNode *buildKDTree(Primitive *prims[], int start, int end, 
                          const AABB& bounds, int depth);

  KDTreeNode *kdtree;
  
  std::vector<Primitive *> primitives;
  std::vector<Primitive *> primitiveIndices;
};


#endif // __KDTREE_H__
