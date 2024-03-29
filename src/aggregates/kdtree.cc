#include <aggregates/kdtree.h> 
#include <util/timer.h>

/** 
 * WARNING:
 *    KD-Tree is broken. The bug is subtle, but if you go to `camera.cc`
 *    and change the line for Non-DOF rays from
 *   
 *        Ray(p, normalized(p-e))
 *    to
 *        Ray(e, normalized(p-e))
 *   
 *    the interesction tests break, and half the scene suddenly disappears.
 *    For instance, in scene `Room` run with DebugShader, the left half of
 *    meshes / scene disappears. With scene `Test` (as of Mar. 8, 2021)
 *    The bottom half of the teapot disappears. This doesn't happen with BVH.
 *   
 *   
 *    For debugging reference, in scene `Test`, run with 1000x1000 resolution,
 *    The pixel (425, 605) should be on the teapot, but no intersection is
 *    registered.
 *   
 *    Git commit hash with test scene as described: 
 *            1e6c2e2af2d2c1881da758276354b2610984ea32
 * 
 * Notes:
 *    
 *   The issue is likely something to do with the following chunk:
 * 
 *      if (tPlane > tMax || tPlane <= 0)
 *        ...
 * 
 *   The issue is not necessarily in this code exactly (which is from PBRT),
 *   but might also be in how things are being set up. I do not think there
 *   is anything wrong with construction, but it might be worth a look.
 */
KDTree::KDTree(std::vector<Primitive *>& prims){
	Timer timer = Timer("Creating KD-Tree: %6lu objects", prims.size()).start();
  
  AABB tbounds;
  for (auto i : prims) 
    tbounds = combine(tbounds, i->bounds);
  bounds = tbounds;

  int maxdepth = 8 + 1.3 * log(prims.size());
  kdtree = build(prims, tbounds, maxdepth);

  timer.stopAndDisplay();
}


KDTreeNode *KDTree::build(std::vector<Primitive *>prims,
                                const AABB& bounds, int depth) {

  if (prims.size() == 0) return NULL;

  KDTreeNode *node = new KDTreeNode();

  int num = prims.size();
  if (num <= 2 || depth == 0) {
    node->numPrims = num;
    node->primOff = m_prims.size();
    for (auto p : prims) {
      m_prims.push_back(p);
    }
    node->axis = -1;
    return node;
  }

  int splitDim = max_index(bounds.max - bounds.min);
  float splitPos = centroid(bounds)[splitDim];

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
  node->a = build(primA, aa, depth - 1);
  node->b = build(primB, bb, depth - 1);
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
  Vec3 invD = 1 / ray.d; // Lets us have slightly faster AABB hits

  struct KDTodo {
    KDTreeNode *node;
    float tMin;
    float tMax;
  };

  float tMin, tMax;
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

    // Leaf node, test against m_prims
    if (cur->numPrims > 0) {

      for (int i = 0; i < cur->numPrims; i++)
        if (m_prims[cur->primOff + i]->hit(ray, rec))
          hit = true;

    // Internal node
    } else {

      KDTreeNode *first, *secnd;
      float tPlane = (cur->splitPos - ray.p[cur->axis]) * invD[cur->axis];
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
