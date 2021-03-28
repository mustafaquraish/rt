#include "core/pointkdtree.h"

using std::vector;

PointKDTree* PointKDTree::newLeaf(vector<Photon> &photons, int start, int end, 
                                  AABB bounds, int depth) {
  PointKDTree *node = new PointKDTree();
  node->dim = -1;
  node->aabb = bounds;
  node->depth = depth;
  for (int i = start; i < end; i++)
    node->photons.push_back(photons[i]);
  return node;
}

void PointKDTree::addPhoton(const Vec &pos, const Colour &col) {
  addPhoton(Photon(pos, col));
}

void PointKDTree::addPhoton(const Photon &pt) {
  // Already split
  if (dim >= 0) {
    bool leftSide = pt.pos[dim] < splitPos;
    if (leftSide) a->addPhoton(pt);
    else          b->addPhoton(pt);
    return;
  }

  photons.push_back(pt);
  aabb.addPoint(pt.pos);

  // Reached terminal depth or node has space, so we're done.
  if (photons.size() < 4 || depth==0) return;
  
  // Oh no, the node is full. Need to split.
  int splitDim = maxIndex(aabb.max - aabb.min);
  int mid = (photons.size()+1)/2;

  std::nth_element(photons.begin(), photons.begin() + mid, photons.end(), 
                    [splitDim](Photon p1, Photon p2) {
                      return p1.pos[splitDim] < p2.pos[splitDim];
                    });

  double splitPos = photons[mid].pos[splitDim];

  AABB boxa = aabb; boxa.max[splitDim] = splitPos;
  AABB boxb = aabb; boxb.min[splitDim] = splitPos;

  this->a = newLeaf(photons,   0,            mid, boxa, depth-1);
  this->b = newLeaf(photons, mid, photons.size(), boxb, depth-1);
  this->dim = splitDim;
  this->splitPos = splitPos;
  this->photons.clear();
}


void PointKDTree::getRange(Vec pt, double range, vector<Photon> &outputPts) {
  if (dim < 0) {
    for (auto p : photons) {
      float dist = length(p.pos-pt);
      if (dist < range) {
        outputPts.push_back(p);
      }
    }
    return;
  }

#if 1
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
#else
  a->getRange(pt, range, outputPts);
  b->getRange(pt, range, outputPts);
#endif

  return;
}