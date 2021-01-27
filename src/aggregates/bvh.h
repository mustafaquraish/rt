#ifndef __BVH_H__
#define __BVH_H__

#include "core/primitive.h"
#include "core/aabb.h"
#include <vector>

struct BVH : Aggregate {
    BVH(std::vector<Primitive *>& list, int start=0, int end=-1);
    ~BVH() { 
      if (!isLeaf) {
        delete bvh_a;
        delete bvh_b;
      }
    }
    bool isLeaf = 0;

    // Using unions here just to be able to access BVH fields easier later.
    union {
      Primitive *a;
      BVH *bvh_a;
    };
    union {
      Primitive *b;
      BVH *bvh_b;
    };

    bool hit(Ray& ray, HitRec& rec);

    inline int height() {
      if (isLeaf) {
        return 1;
      }
      else return max(bvh_a->height() + 1, bvh_b->height()+1);
    }

    inline int num_nodes() {
      if (isLeaf) { 
        return 1;
      }
      else return bvh_a->num_nodes() + bvh_b->num_nodes();
    }
};

#endif // __BVH_H__
