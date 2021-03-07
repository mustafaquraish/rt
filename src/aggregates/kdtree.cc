#include "kdtree.h" 

KDTree::KDTree(std::vector<Primitive *>& prims){
	//TODO
  AABB tbounds;
  for (auto i : prims) 
    tbounds = combine(tbounds, i->bounds);

  int maxdepth = 8 + 1.3 * log(prims.size());
  kdtree = buildKDTree(prims, tbounds, maxdepth);
}


KDTreeNode *KDTree::buildKDTree(std::vector<Primitive *>prims,
                                const AABB& bounds, int depth) {

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

bool KDTree::hit(Ray& ray, HitRec& rec){
	bool hit = false;
  Vec invD = 1 / ray.d; // Lets us have slightly faster AABB hits

  KDTreeNode *stack[128] = {kdtree}; // Holds the node
  int stackPt = 1;                   // Current stack offset

  while (stackPt) {
    KDTreeNode *cur = stack[ --stackPt ];

    // Leaf node, test against primitives
    if (cur->numPrims > 0) {
      for (int i = 0; i < cur->numPrims; i++)
        if (primitives[cur->primOff + i]->hit(ray, rec))
          hit = true;
      
      // if (hit) return true;

    // Internal node
    } else {
      
      if (ray.p[cur->axis] < cur->splitPos) {
        stack[stackPt++] = cur->b;  // Further child
        stack[stackPt++] = cur->a;  //  Closer child
      } else {
        stack[stackPt++] = cur->a;  // Further child
        stack[stackPt++] = cur->b;  //  Closer child
      }
      
    }
  }
  return hit;
}
