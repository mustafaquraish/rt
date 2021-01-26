#include "aggregates/bvh.h"
#include <vector>
#include <algorithm>
#include <time.h>

// This is BAD... I'm not sure how to use it properly yet oops
#define USE_MORTON_CODE 0

#define LOG_BVH_TIME

// Shamelessly stolen from PBRT
inline unsigned int mortonShifts(unsigned int x) {
  if (x == (1 << 10)) --x;
  x = (x | (x << 16)) & 0x30000ff; // x = ---- --98 ---- ---- ---- ---- 7654 3210
  x = (x | (x <<  8)) & 0x300f00f; // x = ---- --98 ---- ---- 7654 ---- ---- 3210
  x = (x | (x <<  4)) & 0x30c30c3; // x = ---- --98 ---- 76-- --54 ---- 32-- --10
  x = (x | (x <<  2)) & 0x9249249; // x = ---- 9--8 --7- -6-- 5--4 --3- -2-- 1--0
  return x;
}

inline unsigned int mortonCode(const Vec &p) {
  unsigned int x = mortonShifts(p.x);
  unsigned int y = mortonShifts(p.y);
  unsigned int z = mortonShifts(p.z);
  return (z << 2) | (y << 1) | x;
}

inline unsigned int mortonCode(const AABB &bb) {
  Vec centroid = (bb.min + bb.max) / 2;
  return mortonCode(centroid);
}

inline unsigned int mortonCode(const Primitive *p) {
  return mortonCode(p->bounds);
}

bool mortonComp(Primitive *a, Primitive *b) {
  return mortonCode(a) < mortonCode(b);
}

namespace bvh_sort {

template <int AXIS>
bool cmpPrim(const Primitive *a, const Primitive *b) {
  AABB ba = a->bounds;
  AABB bb = b->bounds;
  double ca = ba.min[AXIS] + ba.max[AXIS];
  double cb = bb.min[AXIS] + bb.max[AXIS];
  return ca < cb;
}
bool (*funcs[])(const Primitive *, const Primitive *) = {cmpPrim<0>, cmpPrim<1>, cmpPrim<2>};

}

BVH::BVH(std::vector<Primitive *>& prims, int start, int num) {

#ifdef LOG_BVH_TIME 
  bool topLevel = (num == -1);
  clock_t timeBegin;
  if (topLevel) {
    printf("[+] Creating BVH from %lu objects...\n", prims.size());
    timeBegin = clock();
  }
#endif

  // First iteration, sort the array first and set parameters correctly.
  if (num == -1) {
    if (USE_MORTON_CODE) std::sort(prims.begin(), prims.end(), mortonComp);
    num = prims.size();
    topLevel = true;
  }

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

  if (!USE_MORTON_CODE) {
    int dim = rand() % 3;
    int mid = (num + 1) / 2;
    std::nth_element(prims.begin()+start, 
                     prims.begin()+start+mid,
                     prims.begin()+start+num, 
                     bvh_sort::funcs[dim]);
  }
  
  int midpt = start + (num + 1) / 2;
  a = new BVH(prims, start, (num + 1) / 2);
  b = new BVH(prims, midpt, num / 2);
  bounds = combine(a->bounds, b->bounds);

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