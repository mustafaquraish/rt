/**
 * Simple one-header implementation of a Point-KDTree.
 * 
 * To use this, the templated type must have a `pos` field of type `Vec3`, for
 * instance as follows:
 * 
 *        struct KDElem_t {
 *           Vec3 pos;
 *           <other fields you want>
 *        };
 *        
 *        PointKDTree<KDElem_t> *tree = new PointKDTree<KDElem_t>();
 *        tree->addPoint({pos, <other fields>});
 * 
 * (c) Mustafa Quraish, 2021
 */

#pragma once

#include <core/math.h>
#include <core/aabb.h>

#include <vector>

template <typename ValType>
struct PointKDTree {
  std::vector<ValType> points;
  double splitPos;
  int dim = -1;
  int depth = 20;

  PointKDTree() {};
  PointKDTree(int depth) : depth(depth) {};

  PointKDTree *a = NULL;
  PointKDTree *b = NULL;
  
  AABB aabb;
  
  ~PointKDTree() {
    if (dim < 0) {
      delete a;
      delete b;
    }
  }

  static PointKDTree* newLeaf(std::vector<ValType> &points, int start, int end,
                              AABB bounds, int depth) {
    PointKDTree *node = new PointKDTree();
    node->dim = -1;
    node->aabb = bounds;
    node->depth = depth;
    for (int i = start; i < end; i++)
      node->points.push_back(points[i]);
    return node;
  }
  
  void getRange(Vec3 pt, double range, std::vector<ValType> &outputPts) {
    if (dim < 0) {
      for (auto p : points) {
        float dist = length(p.pos-pt);
        if (dist < range) {
          outputPts.push_back(p);
        }
      }
      return;
    }

    float off =  pt[dim] - splitPos;
    if (abs(off) <= range) {
      a->getRange(pt, range, outputPts);
      b->getRange(pt, range, outputPts);
    } else {
      if (pt[dim] < splitPos)
        a->getRange(pt, range, outputPts);
      else
        b->getRange(pt, range, outputPts);
    }

    return;
  }

  template<typename... Args>
  void emplacePoint(Args&&... args) {
    addPoint(ValType(std::forward<Args>(args)...));
  }

  void addPoint(const ValType &pt) {
      // Already split
    if (dim >= 0) {
      bool leftSide = pt.pos[dim] < splitPos;
      if (leftSide) a->addPoint(pt);
      else          b->addPoint(pt);
      return;
    }

    points.push_back(pt);
    aabb.addPoint(pt.pos);

    // Reached terminal depth or node has space, so we're done.
    if (points.size() < 4 || depth==0) return;
    
    // Oh no, the node is full. Need to split.
    int splitDim = maxIndex(aabb.max - aabb.min);
    int mid = (points.size()+1)/2;

    std::nth_element(points.begin(), points.begin() + mid, points.end(), 
                      [splitDim](ValType p1, ValType p2) {
                        return p1.pos[splitDim] < p2.pos[splitDim];
                      });

    double splitPos = points[mid].pos[splitDim];

    AABB boxa = aabb; boxa.max[splitDim] = splitPos;
    AABB boxb = aabb; boxb.min[splitDim] = splitPos;

    this->a = newLeaf(points,   0,            mid, boxa, depth-1);
    this->b = newLeaf(points, mid, points.size(), boxb, depth-1);
    this->dim = splitDim;
    this->splitPos = splitPos;
    this->points.clear();
  }
};



