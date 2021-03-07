#include "kdtree.h" 

KDTree::KDTree(std::vector<Primitive *>& prims){
	//TODO
  AABB bounds;
  for (auto i : prims) 
    bounds = combine(bounds, i->bounds);

  int maxdepth = 8 + 1.3 * log(prims.size());
  kdtree = buildKDTree(&prims[0], 0, prims.size(), bounds, maxdepth);
  primitives = prims;
}


KDTreeNode *KDTree::buildKDTree(Primitive *prims[], int start, int end, 
                                const AABB& bounds, int depth) {

  KDTreeNode *node = new KDTreeNode();

  int num = end - start;
  if (num <= 4 || depth == 0) {
    node->numPrims = num;
    node->primOff = start;
    node->axis = -1;
    return node;
  }

  int splitDim = maxIndex(bounds.max - bounds.min);
  double splitPos = centroid(bounds)[splitDim];
  
  Primitive **partitioned = new Primitive*[num];
  int curIdx = 0;

  AABB aa = bounds; aa.max[splitDim] = splitPos;
  AABB bb = bounds; bb.min[splitDim] = splitPos;

  for (int i = start; i < end; i++)
    if (overlap(prims[i]->bounds, aa) && !overlap(prims[i]->bounds, bb))
      partitioned[curIdx++] = prims[i];
  int p1 = curIdx;
  for (int i = start; i < end; i++)
    if (overlap(prims[i]->bounds, aa) && overlap(prims[i]->bounds, bb))
      partitioned[curIdx++] = prims[i];
  int p2 = curIdx;
  for (int i = start; i < end; i++)
    if (!overlap(prims[i]->bounds, aa) && overlap(prims[i]->bounds, bb))
      partitioned[curIdx++] = prims[i];

  for (int i = start; i < end; i++) {
    prims[i] = partitioned[i];
  }
  
  delete[] partitioned;

  node->axis = splitDim;
  node->splitPos = splitPos;
  node->numPrims = 0;
  node->a = buildKDTree(prims, start, p2, aa, depth - 1);
  node->b = buildKDTree(prims, p1, end, bb, depth - 1);
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
      
      if (hit) return true;

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
