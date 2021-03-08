#include "kdtree.h" 
#include "util/timer.h"

KDTree::KDTree(std::vector<Primitive *>& prims){
	Timer timer = Timer("Creating KD-Tree: %6lu objects", prims.size()).start();
  
  AABB tbounds;
  for (auto i : prims) 
    tbounds = combine(tbounds, i->bounds);
  bounds = tbounds;

  int maxdepth = 8 + 1.3 * log(prims.size());
  kdtree = buildKDTree(prims, tbounds, maxdepth);

  timer.stopAndDisplay();
}


KDTreeNode *KDTree::buildKDTree(std::vector<Primitive *>prims,
                                const AABB& bounds, int depth) {

  if (prims.size() == 0) return NULL;

  KDTreeNode *node = new KDTreeNode();

  int num = prims.size();
  if (num <= 4 || depth == 0) {
    node->numPrims = num;
    node->primOff = primitives.size();
    for (auto p : prims) {
      primitives.push_back(p);
    }
    node->axis = -1;
    return node;
  }

  int splitDim = maxIndex(bounds.max - bounds.min);
  double splitPos = centroid(bounds)[splitDim];

  AABB aa = bounds; aa.max[splitDim] = splitPos;
  AABB bb = bounds; bb.min[splitDim] = splitPos;

  std::vector<Primitive *>primA;
  std::vector<Primitive *>primB;

  for (auto i : prims) {
    if (overlap(i->bounds, aa)) primA.push_back(i);
    if (overlap(i->bounds, bb)) primB.push_back(i);
  }

  node->axis = splitDim;
  node->splitPos = splitPos;
  node->numPrims = 0;
  node->a = buildKDTree(primA, aa, depth - 1);
  node->b = buildKDTree(primB, bb, depth - 1);
  return node;
}

#define PUSH(v, tmin, tmax)                                                    \
  if (v) {                                                                     \
    stack[stackPt].node = v;                                                   \
    stack[stackPt].tMin = tmin;                                                \
    stack[stackPt].tMax = tmax;                                                \
    stackPt++;                                                                 \
  } else {}

bool KDTree::hit(Ray& ray, HitRec& rec) {
	bool hit = false;
  Vec invD = 1 / ray.d; // Lets us have slightly faster AABB hits

  struct KDTodo {
    KDTreeNode *node;
    double tMin;
    double tMax;
  };

  double tMin, tMax;
  if (!bounds.hit(ray, tMin, tMax, invD))
    return false;

  KDTodo stack[128] = { { kdtree, tMin, tMax }  }; // Holds the node
  int stackPt = 1;                   // Current stack offset

  while (stackPt) {
    --stackPt;
    KDTreeNode *cur = stack[stackPt].node;
    tMin = stack[stackPt].tMin;
    tMax = stack[stackPt].tMax;

    if (ray.tMax < tMin) break;

    // Leaf node, test against primitives
    if (cur->numPrims > 0) {

      for (int i = 0; i < cur->numPrims; i++)
        if (primitives[cur->primOff + i]->hit(ray, rec))
          hit = true;

    // Internal node
    } else {

      KDTreeNode *first, *secnd;
      double tPlane = (cur->splitPos - ray.p[cur->axis]) * invD[cur->axis];
      bool below = (ray.p[cur->axis] < cur->splitPos) ||
                   (ray.p[cur->axis] < cur->splitPos && ray.d[cur->axis] <= 0);
      
      first = below ? cur->a : cur->b;
      secnd = below ? cur->b : cur->a;

      if (tPlane > tMax || tPlane <= 0) {
        PUSH(first, tMin, tMax);
      } else if (tPlane < tMin) {
        PUSH(secnd, tMin, tMax);
      } else {
        PUSH(secnd, tPlane, tMax);
        PUSH(first, tMin, tPlane);
      }
    }
  }
  return hit;
}

#undef PUSH
