#include <aggregates/bvh.h>
#include <util/timer.h>

#include <algorithm>
#include <cassert>

int splitEqualCounts(std::vector<Primitive *>& prims, int start, int end,
                AABB& bounds, int dim) {
  int mid = (start + end + 1) / 2;
  std::nth_element(&prims[start], &prims[mid], &prims[end], 
                   [dim](Primitive *a, Primitive *b) {
                      AABB ba = a->bounds, bb = b->bounds;
                      float ca = ba.min[dim] + ba.max[dim];
                      float cb = bb.min[dim] + bb.max[dim];
                      return ca < cb;
                   });
  return mid;
}

int splitSAH(std::vector<Primitive *>& prims, int start, int end,
        AABB& bounds, int dim) {
  constexpr int nBuckets = 12;
  struct { int count = 0; AABB bounds; } buckets[nBuckets];

  for (int i = start; i < end; ++i) {
    int b = nBuckets * bounds.offset(centroid(prims[i]->bounds))[dim];
    if (b == nBuckets) b = nBuckets - 1;
    buckets[b].count++;
    buckets[b].bounds = combine(buckets[b].bounds, prims[i]->bounds);
  }

  float cost[nBuckets - 1];
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
                                Vec3 off = bounds.offset(centroid(p->bounds));
                                int bucket = nBuckets * off[dim];
                                if (bucket == nBuckets) bucket = nBuckets - 1;
                                return bucket <= minBucket;
                             });
  int mid = pmid - &prims[0];

  // TODO: this is bad, but currently no support for multiple objects in node
  if (mid == start || mid == end) 
    mid = splitEqualCounts(prims, start, end, bounds, dim);
  
  return mid;
}

BVHTree *BVH::build(std::vector<Primitive *>& list, int start, int end) {
  BVHTree *node = new BVHTree();
  
  AABB bounds;
  for (int i = start; i < end; i++) 
    bounds = combine(bounds, list[i]->bounds);

  int num = end - start;
  if (num <= 1) {
    node->primOff = start; 
    node->numPrims = num;
    node->bounds = bounds;
    return node;
  }
  
  int dim = max_index(bounds.max - bounds.min);

  int mid;  
  switch (m_method) {
    case SAH: mid = splitSAH(list, start, end, bounds, dim); break;
    case Median: mid = splitEqualCounts(list, start, end, bounds, dim); break;
  }

  node->a = build(list, start, mid);
  node->b = build(list, mid, end);
  node->bounds = bounds;
  node->numPrims = 0;
  return node;
}

int BVH::flatten(BVHTree *root) {
  int curIdx = m_nodes.size();
  BVHLinear cur;

  cur.bounds = root->bounds;
  cur.numPrims = root->numPrims;
  cur.primOff = root->primOff;
  m_nodes.push_back(cur);

  if (root->numPrims == 0) { // Internal node
    /* ignore this return  */   flatten(root->a);
    m_nodes[curIdx].child2Off = flatten(root->b);
  }
  return curIdx;
}

BVH::BVH(std::vector<Primitive *>& prims, BVH::BuildMethod method)
    : m_method(method) {
  Timer timer = Timer("Creating BVH from %6lu objects", prims.size()).start();

  BVHTree *bvh = build(prims, 0, prims.size());
  bounds = bvh->bounds;
  m_prims = prims;
  flatten(bvh);

  timer.stopAndDisplay();
}

bool BVH::hit(Ray& ray, HitRec& rec) {
  bool hit = false;
  Vec3 invD = 1 / ray.d; // Lets us have slightly faster AABB hits

  // Stack initially begins with root node, index `0`
  int stack[128] = {0}; // Holds the indices of the m_nodes to visit
  int stackPt = 1;      // Current stack offset

  while (stackPt) {
    int curIdx = stack[ --stackPt ];
    BVHLinear *cur = &m_nodes[ curIdx ];

    // Leaf node, test against m_prims
    if (cur->numPrims > 0) {
      for (int i = 0; i < cur->numPrims; i++)
        if (m_prims[cur->primOff + i]->hit(ray, rec))
          hit = true;

    // Internal node
    } else {
      int a = curIdx + 1, b = cur->child2Off;
      float a1, a2, b1, b2;

      // Check both children's bounding boxes
      int hit_a = m_nodes[a].bounds.hit(ray, a1, a2, invD);
      int hit_b = m_nodes[b].bounds.hit(ray, b1, b2, invD);

      if (!hit_a && !hit_b) continue;  // Hit none of them
      if (!hit_b) { stack[stackPt++] = a; continue; } // Didn't hit b, try a
      if (!hit_a) { stack[stackPt++] = b; continue; } // Didn't hit a, try b

      stack[stackPt++] = a1 >= b1 ? a : b;  // Further child
      stack[stackPt++] = a1 <  b1 ? a : b;  //  Closer child
    }
  }
  return hit;
}