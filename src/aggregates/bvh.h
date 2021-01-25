#ifndef __BVH_H__
#define __BVH_H__

#include "core/primitive.h"
#include "core/aabb.h"
#include <vector>

struct BVH : Aggregate {
    BVH(std::vector<Primitive *>& list, int start=0, int num=-1);
    ~BVH() { 
      if (!isLeaf) {
        delete (BVH *)a;
        delete (BVH *)b;
      }
    }
    AABB bounds;
    bool isLeaf = 0;
    Primitive *a;
    Primitive *b;
    
    bool hit(Ray& ray, HitRec& rec);
    AABB getBounds() const { return bounds; }

    inline int height() {
      if (isLeaf) {
        return 1;
      }
      else return max(((BVH*)a)->height() + 1, ((BVH*)b)->height()+1);
    }

    inline int num_nodes() {
      if (isLeaf) { 
        return 1;
      }
      else return ((BVH*)a)->num_nodes() + ((BVH*)b)->num_nodes();
    }
};

#endif // __BVH_H__
