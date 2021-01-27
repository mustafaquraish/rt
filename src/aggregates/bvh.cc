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

  // TODO: this is bad, but currently no support for multiple objects in node
  if (mid == start || mid == end) 
    mid = equalCounts(prims, start, end, totalBounds, dim);
  
  return mid;
}

BVH::BVH(std::vector<Primitive *>& prims, int start, int end) {  
// printf("called with %d %d\n", start, end);
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
    // b = NULL;
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
  // int mid = surfaceAreaHueristic(prims, start, end, totalBounds, dim);
  int mid = equalCounts(prims, start, end, totalBounds, dim);  
  
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
    // Intersect both children
    HitRec ra, rb;
    bool hit_a = a->hit(ray, rec);
    bool hit_b = b->hit(ray, rec);
    return hit_a || hit_b;
  }

  // Check both children's bounding boxes
  double a1, a2, b1, b2;
  int hit_a = a->bounds.hit(ray, a1, a2);
  int hit_b = b->bounds.hit(ray, b1, b2);

  if (!hit_a && !hit_b) return false;  // Hit none of them
  if (!hit_b) return a->hit(ray, rec); // Didn't hit b, recurse on a
  if (!hit_a) return b->hit(ray, rec); // Didn't hit a, recurse on b

  // OK, we hit both children. Want to see which bounding box is 
  // closer to the ray
  HitRec r1, r2;
  Primitive *first = a1 <  b1 ? a : b;  // `first` is the child starting closer
  Primitive *secnd = a1 >= b1 ? a : b;  // 'secnd` is the child starting after

  int hit_first = first->hit(ray, rec);
  int hit_secnd = secnd->hit(ray, rec);
  
  return hit_first || hit_secnd;
}