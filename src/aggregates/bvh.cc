#include "util/timer.h"
#include "aggregates/bvh.h"
#include <algorithm>

int equalCounts(std::vector<Primitive *>& prims, int start, int end,
                AABB& bounds, int dim) {
  int mid = (start + end + 1) / 2;
  std::nth_element(&prims[start], &prims[mid], &prims[end], 
                   [dim](Primitive *a, Primitive *b) {
                      AABB ba = a->bounds, bb = b->bounds;
                      double ca = ba.min[dim] + ba.max[dim];
                      double cb = bb.min[dim] + bb.max[dim];
                      return ca < cb;
                   });
  return mid;
}

int SAH(std::vector<Primitive *>& prims, int start, int end,
        AABB& bounds, int dim) {
  constexpr int nBuckets = 12;
  struct { int count = 0; AABB bounds; } buckets[nBuckets];

  for (int i = start; i < end; ++i) {
    int b = nBuckets * bounds.offset(centroid(prims[i]->bounds))[dim];
    if (b == nBuckets) b = nBuckets - 1;
    buckets[b].count++;
    buckets[b].bounds = combine(buckets[b].bounds, prims[i]->bounds);
  }

  double cost[nBuckets - 1];
  for (int i = 0; i < nBuckets - 1; ++i) {
    AABB b0, b1;
    int c0 = 0, c1 = 0;
    for (int j = 0; j <= i; ++j)
      b0 = combine(b0, buckets[j].bounds), c0 += buckets[j].count;
    for (int j = i+1; j < nBuckets; ++j)
      b1 = combine(b1, buckets[j].bounds), c1 += buckets[j].count;
    cost[i] = .125 + (c0 * area(b0) + c1 * area(b1)) / area(bounds);
  }

  int minBucket = 0;
  for (int i = 1; i < nBuckets - 1; ++i)
    if (cost[i] < cost[minBucket])
      minBucket = i;
  
  auto pmid = std::partition(&prims[start], &prims[end],
                             [=](const Primitive* p) {
                                Vec off = bounds.offset(centroid(p->bounds));
                                int bucket = nBuckets * off[dim];
                                if (bucket == nBuckets) bucket = nBuckets - 1;
                                return bucket <= minBucket;
                             });
  int mid = pmid - &prims[0];

  // TODO: this is bad, but currently no support for multiple objects in node
  if (mid == start || mid == end) 
    mid = equalCounts(prims, start, end, bounds, dim);
  
  return mid;
}

BVHTree *BVH::buildBVH(std::vector<Primitive *>& prims, int start, int end) {
  BVHTree *node = new BVHTree();
  
  AABB bounds;
  for (int i = start; i < end; i++) 
    bounds = combine(bounds, prims[i]->bounds);

  int num = end - start;
  if (num <= 1) {
    node->primOff = start; 
    node->numPrims = num;
    node->bounds = bounds;
    return node;
  }
  
  int dim = maxIndex(bounds.max - bounds.min);
  
  int mid = SAH(prims, start, end, bounds, dim);
  // int mid = equalCounts(prims, start, end, bounds, dim);
  
  node->a = buildBVH(prims, start, mid);
  node->b = buildBVH(prims, mid, end);
  node->bounds = bounds;
  node->numPrims = 0;
  return node;
}

int BVH::flatten(BVHTree *root) {
  int curIdx = nodes.size();
  BVHLinear cur;

  cur.bounds = root->bounds;
  cur.numPrims = root->numPrims;
  cur.primOff = root->primOff;
  nodes.push_back(cur);

  if (root->numPrims == 0) { // Internal node
    /* ignore this return  */ flatten(root->a);
    nodes[curIdx].child2Off = flatten(root->b);
  }
  return curIdx;
}

BVH::BVH(std::vector<Primitive *>& prims) {
  char message[512];
  sprintf(message, "Creating BVH from %6lu objects", prims.size());
  Timer timer = Timer(message).start();

  BVHTree *bvh = buildBVH(prims, 0, prims.size());
  bounds = bvh->bounds;
  primitives = prims;
  flatten(bvh);

  timer.stopAndDisplay();
}

bool BVH::hit(Ray& ray, HitRec& rec) {
  bool hit = false;
  Vec invD = 1 / ray.d; // Lets us have slightly faster AABB hits

  // Stack initially begins with root node, index `0`
  int stack[128] = {0}; // Holds the indices of the nodes to visit
  int stackPt = 1;      // Current stack offset

  while (stackPt) {
    int curIdx = stack[ --stackPt ];
    BVHLinear *cur = &nodes[ curIdx ];

    // Leaf node, test against primitives
    if (cur->numPrims > 0) {
      for (int i = 0; i < cur->numPrims; i++)
        if (primitives[cur->primOff + i]->hit(ray, rec))
          hit = true;

    // Internal node
    } else {
      int a = curIdx + 1, b = cur->child2Off;
      double a1, a2, b1, b2;

      // Check both children's bounding boxes
      int hit_a = nodes[a].bounds.hit(ray, a1, a2, invD);
      int hit_b = nodes[b].bounds.hit(ray, b1, b2, invD);

      if (!hit_a && !hit_b) continue;  // Hit none of them
      if (!hit_b) { stack[stackPt++] = a; continue; } // Didn't hit b, try a
      if (!hit_a) { stack[stackPt++] = b; continue; } // Didn't hit a, try b

      stack[stackPt++] = a1 >= b1 ? a : b;  // Further child
      stack[stackPt++] = a1 <  b1 ? a : b;  //  Closer child
    }
  }
  return hit;
}