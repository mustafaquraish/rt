#ifndef __PHOTONMAP_H__
#define __PHOTONMAP_H__

#include "core/math.h"
#include "core/aabb.h"

#include <vector>

struct Photon {
  Vec pos;
  Colour col;

  Photon(const Vec& pos, const Colour& col): pos(pos), col(col) {};
};

struct PointKDTree {
  std::vector<Photon> photons;
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

  static PointKDTree* newLeaf(std::vector<Photon> &photons, int start, int end,
                              AABB bounds, int depth);
  
  void getRange(Vec pt, double range, std::vector<Photon> &photonOutput);

  void addPhoton(const Photon &pt);
  void addPhoton(const Vec &pos, const Colour &col);
};



#endif // __PHOTONMAP_H__
