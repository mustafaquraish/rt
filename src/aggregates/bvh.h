#ifndef __BVH_H__
#define __BVH_H__

#include "core/primitive.h"
#include "core/aabb.h"
#include <vector>

struct BVH : Aggregate {
    BVH(std::vector<Primitive *>& list, int start=0, int end=-1);
    
    ~BVH() { 
      delete b;
      delete a;
    }

    bool isLeaf = 0;

    // Using unions here just to be able to access BVH fields easier later.
    union {
      struct { Primitive *a, *b; };
      struct { BVH *bva, *bvb; };
    };

    bool hit(Ray& ray, HitRec& rec);

    inline int height() {
      if (isLeaf) return 1;
      return max(bva->height(), bvb->height()) + 1;
    }

    inline int num_nodes() {
      if (isLeaf) return 1;
      return bva->num_nodes() + bvb->num_nodes();
    }
};

#endif // __BVH_H__
