#include "aggregates/bvh.h"
#include <vector>
#include <algorithm>
#include <time.h>

#define LOG_BVH_TIME

int equalCounts(std::vector<Primitive *>& prims, int start, int end,
                AABB& totalBounds, int dim) {
  int mid = (end + start + 1) / 2;
  std::nth_element(prims.begin() + start, prims.begin() + mid,
                   prims.begin() + end, 
                   [dim](const Primitive *a, const Primitive *b) {
                     AABB ba = a->bounds;
                     AABB bb = b->bounds;
                     double ca = ba.min[dim] + ba.max[dim];
                     double cb = bb.min[dim] + bb.max[dim];
                     return ca < cb;
                   });
  return mid;
}

// TODO: Make this work with multiple objects inside a BVHNode
//          It is currenty too slow for big scenes
int surfaceAreaHueristic(std::vector<Primitive *>& prims, int start, int end,
                         AABB& totalBounds, int dim) {
  // printf("called with %d %d\n", start, end);
  constexpr int nBuckets = 12;
  struct BucketInfo {
    int count = 0;
    AABB bounds;
  };
  BucketInfo buckets[nBuckets];

  for (int i = start; i < end; ++i) {
    int b = nBuckets * totalBounds.offset(centroid(prims[i]->bounds))[dim];
    if (b == nBuckets) b = nBuckets - 1;
    buckets[b].count++;
    buckets[b].bounds = combine(buckets[b].bounds, prims[i]->bounds);
  }

  double cost[nBuckets - 1];
  for (int i = 0; i < nBuckets - 1; ++i) {
    AABB b0, b1;
    int count0 = 0, count1 = 0;
    for (int j = 0; j <= i; ++j) {
      b0 = combine(b0, buckets[j].bounds);
      count0 += buckets[j].count;
    }
    for (int j = i+1; j < nBuckets; ++j) {
      b1 = combine(b1, buckets[j].bounds);
      count1 += buckets[j].count;
    }
    cost[i] = .125f + (count0 * area(b0) + count1 * area(b1)) / area(totalBounds);
  }

  double minCost = cost[0];
  int minCostSplitBucket = 0;
  for (int i = 1; i < nBuckets - 1; ++i) {
      if (cost[i] < minCost) {
          minCost = cost[i];
          minCostSplitBucket = i;
      }
  }
  
  auto pmid = std::partition(prims.begin() + start, 
                             prims.begin() + end, 
                             [=](const Primitive* pi) {
                               Vec c = centroid(pi->bounds);
                               Vec off = totalBounds.offset(c);
                               int b = nBuckets * off[dim];
                               if (b == nBuckets) b = nBuckets - 1;
                               return b <= minCostSplitBucket;
                             });
  int mid = pmid - prims.begin();

  // TODO: this is bad, but currently no support for multiple objects in node
  if (mid == start || mid == end) 
    mid = equalCounts(prims, start, end, totalBounds, dim);
  
  return mid;
}

BVHTree *BVH::buildBVH(std::vector<Primitive *>& prims, int start, int end) {
//  printf("called with %d %d\n", start, end);
  BVHTree *node = new BVHTree();
  
  int num = end - start;
  // Now onto the regular recursive building...

  AABB totalBounds = prims[start]->bounds;
  for (int i = start; i < end; i++) {
    totalBounds = combine(totalBounds, prims[i]->bounds);
  }

  if (num <= 2) {
    node->offset = start; 
    node->numPrims = num;
    node->bounds = totalBounds;
    node->isLeaf = 1;
    node->axis = -1;
    return node;
  }
  
  int dim = maxIndex(totalBounds.max - totalBounds.min);
  int mid = surfaceAreaHueristic(prims, start, end, totalBounds, dim);
//  int mid = equalCounts(prims, start, end, totalBounds, dim);
  
  node->bva = buildBVH(prims, start, mid);
  node->bvb = buildBVH(prims, mid, end);
  node->axis = dim;
  node->bounds = totalBounds;

  return node;
}

int BVH::flatten(BVHTree *root) {
  int curIdx = nodes.size();

  BVHLinear cur;

  cur.bounds = root->bounds;
  if (root->axis < 0) {
    cur.primOff = root->offset;
    cur.nPrimitives = root->numPrims;
    cur.axis = -1;
    nodes.push_back(cur);
    return curIdx;
  }
  
  cur.nPrimitives = 0;
  nodes.push_back(cur);

  /*   ignore  */ flatten(root->bva);
  int child2off = flatten(root->bvb);

  nodes[curIdx].child2Off = child2off;

  return curIdx;
}

BVH::BVH(std::vector<Primitive *>& prims, int start, int end) {  
  printf("[+] Creating BVH from %lu objects", prims.size());
  clock_t timeBegin = clock();

  bvh = buildBVH(prims, 0, prims.size());
  bounds = bvh->bounds;
  primitives = prims;

  flatten(bvh);

  printf("\nHave %ld nodes in BVH\n", nodes.size());

  clock_t timeEnd = clock();
  double buildTime = (double)(timeEnd - timeBegin) / CLOCKS_PER_SEC;
  printf(": %.3fs\n", buildTime);

  return;
}

bool BVH::hit(Ray& ray, HitRec& rec) {
  // return bvh->hit(ray, rec, primitives);

  int stack[1024];
  int stackPt = 0;

  stack[stackPt++] = 0;

  int dirIsNeg[3] = {ray.d.x < 0, ray.d.y < 0, ray.d.z < 0};
  bool hit = false;

  while (stackPt) {
    int curIdx = stack[ --stackPt ];
    BVHLinear *cur = &nodes[ curIdx ];

    if (cur->nPrimitives > 0) {

      for (int i = 0; i < cur->nPrimitives; i++)
        if (primitives[cur->primOff + i]->hit(ray, rec))
          hit = true;

    } else {
      
      if (cur->bounds.hit(ray)) {
        if (dirIsNeg[cur->axis]) {
          stack[stackPt++] = curIdx + 1;
          stack[stackPt++] = cur->child2Off;

        } else {
          stack[stackPt++] = cur->child2Off;
          stack[stackPt++] = curIdx + 1;
        }
      }
      
    }
  }
  return hit;
}

bool BVHTree::hit(Ray& ray, HitRec& rec, std::vector<Primitive *>& prims) {
  if (isLeaf) {
    bool hit = false;
    for (int i = offset; i < offset + numPrims; i++)
      if (prims[i]->hit(ray, rec))
        hit = true;
    return hit;
  }

  // Check both children's bounding boxes
  double a1, a2, b1, b2;
  int hit_a = bva->bounds.hit(ray, a1, a2);
  int hit_b = bvb->bounds.hit(ray, b1, b2);

  if (!hit_a && !hit_b) return false;  // Hit none of them
  if (!hit_b) return bva->hit(ray, rec, prims); // Didn't hit b, recurse on a
  if (!hit_a) return bvb->hit(ray, rec, prims); // Didn't hit a, recurse on b

  // OK, we hit both children. Want to see which bounding box is 
  // closer to the ray
  HitRec r1, r2;
  BVHTree *first = a1 <  b1 ? bva : bvb;  // `first` is the child starting closer
  BVHTree *secnd = a1 >= b1 ? bva : bvb;  // 'secnd` is the child starting after

  int hit_first = first->hit(ray, rec, prims);
  int hit_secnd = secnd->hit(ray, rec, prims);
  
  return hit_first || hit_secnd;
}