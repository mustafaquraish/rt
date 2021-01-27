#include "aggregates/bvh.h"
#include <vector>
#include <algorithm>
#include <time.h>

#define LOG_BVH_TIME

// TODO: Make this work with multiple objects inside a BVHNode
int surfaceAreaHueristic(std::vector<Primitive *>& prims, int start, int end,
                         AABB& totalBounds, int dim) {
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
  
  auto pmid = std::partition(prims.begin() + start, prims.begin() + end, 
                             [=](const Primitive* pi) {
                               Vec c = centroid(pi->bounds);
                               Vec off = totalBounds.offset(c);
                               int b = nBuckets * off[dim];
                               if (b == nBuckets) b = nBuckets - 1;
                               return b <= minCostSplitBucket;
                             });
  int mid = pmid - prims.begin();
  if (mid == start) mid++;
  if (mid == end) mid--;
  return mid;
}

int equalCounts(std::vector<Primitive *>& prims, int start, int end,
                AABB& totalBounds, int dim) {
  int mid = start + ((end - start) + 1) / 2;
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

BVH::BVH(std::vector<Primitive *>& prims, int start, int end) {  

#ifdef LOG_BVH_TIME 
  bool topLevel = (end == -1);
  clock_t timeBegin;
  if (topLevel) {
    printf("[+] Creating BVH from %lu objects...\n", prims.size());
    timeBegin = clock();
  }
#endif

  // First iteration, sort the array first and set parameters correctly.
  if (end == -1) {
    end = prims.size();
  }

  int num = end - start;
  // Now onto the regular recursive building...

  if (num == 1) {
    a = b = prims[start];
    bounds = a->bounds;
    b = NULL;
    isLeaf = 1;
    return;

  } else if (num == 2) {
    a = prims[start]; 
    b = prims[start+1];
    bounds = combine(a->bounds, b->bounds);
    isLeaf = 1;
    return;
  }

  AABB totalBounds = prims[start]->bounds;
  for (int i = start; i < end; i++)
    totalBounds = combine(totalBounds, prims[i]->bounds);
  
  int dim = maxIndex(totalBounds.max - totalBounds.min);
  // int dim = rand() % 3;

  // Split algorithm:
  int mid = surfaceAreaHueristic(prims, start, end, totalBounds, dim);
  // int mid = equalCounts(prims, start, end, totalBounds, dim);  
  
  a = new BVH(prims, start, mid);
  b = new BVH(prims, mid, end);
  bounds = totalBounds;

#ifdef LOG_BVH_TIME 
  if (topLevel) {
    clock_t timeEnd = clock();
    double buildTime = (double)(timeEnd - timeBegin) / CLOCKS_PER_SEC;
    printf("[+] BVH Constructed in %.3fs\n", buildTime);
  }
#endif
  return;
}

bool BVH::hit(Ray& ray, HitRec& rec) {
  if (isLeaf) {
    // If we have only one child...
    if (b == NULL) return a->hit(ray, rec);
    // Intersect both children
    HitRec ra, rb;
    bool hit_a = a->hit(ray, ra);
    bool hit_b = b->hit(ray, rb);
    // Return closer one if there were intersections
    if (!hit_a && !hit_b) return false;
    if      (!hit_a) rec = rb;
    else if (!hit_b) rec = ra;
    else             rec = (ra.t1 < rb.t1) ? ra : rb;
    return true;
  }

  // Check both children's bounding boxes
  double a1, a2, b1, b2;
  int hit_a = a->bounds.hit(ray, a1, a2);
  int hit_b = b->bounds.hit(ray, b1, b2);

  if (!hit_a && !hit_b) return false;           // Hit none of them
  if (!hit_b) return hit_a && a->hit(ray, rec); // Didn't hit b, recurse on a
  if (!hit_a) return hit_b && b->hit(ray, rec); // Didn't hit a, recurse on b

  // OK, we hit both children. Want to see which bounding box is 
  // closer to the ray
  HitRec r1, r2;
  Primitive *first = a1 <  b1 ? a : b;  // `first` is the child starting closer
  Primitive *secnd = a1 >= b1 ? a : b;  // 'secnd` is the child starting after

  // Swap aX and bX so a1/2 corresponds to `first`
  if (a1 > b1) {
    b1 = a1;
  }

  int hit_first = first->hit(ray, r1);
  
  // We have the following scenario, we can completely discard the 2nd child.
  //                  a1      `first`       a2
  //  ray             |---------------------|
  // ------->                  X                `secnd`    
  //                  hit-----/          |-----------------| 
  //                                    b1                b2
  if (hit_first && r1.t1 < b1) {
    rec = r1;
    return true;
  }

  int hit_secnd = secnd->hit(ray, r2);
  
  // Missed both children
  if (!hit_first && !hit_secnd) return false;
  
  // Pick the closest intersection:
  if      (!hit_secnd) rec = r1;
  else if (!hit_first) rec = r2;
  else                 rec = (r1.t1 < r2.t1) ? r1 : r2;

  return true;
}