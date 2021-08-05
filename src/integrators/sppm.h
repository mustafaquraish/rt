#ifndef __SPPM_H__
#define __SPPM_H__

#include "core/integrator.h"
#include "util/params.h"
#include "core/primitive.h"
#include "core/pointkdtree.h"

#include <atomic>

struct SPPM : Integrator {
  SPPM(RenderParams params) : Integrator(params) {
    if (params.exists("numPhotons"))
      numPhotonsPerIter = params.get<int>("numPhotons");
    if (params.exists("numIterations"))
      numIterations = params.get<int>("numIterations");
    if (params.exists("photonRange"))
      photonRange = params.get<double>("photonRange");
  };

  void addVisiblePoint(const Ray& ray, Scene *scene, RNG& rng, int pixelIdx);
  void sendPhoton(Scene *scene, RNG &rng);
  virtual void render(Scene *scene);

  struct Photon {
    Vec pos, dir;
    Colour col;
    Colour acc = Colour(0);
    int M = 0;
    Photon(Vec pos, Vec dir, Vec col) : pos(pos), dir(dir), col(col), M(0) {};
  };

  struct PhotonNode {
    Vec pos;
    Photon *photon;
    PhotonNode() {};
    PhotonNode(Vec pos, Photon *photon): pos(pos), photon(photon) {};
  };

  const int depth = 10;
  int numIterations = 10;
  int numPhotonsPerIter = 10000;
  double photonRange = .2; // ??
  std::vector<Photon> photons;
  PointKDTree<PhotonNode> *photonMap;
};

#endif // __SPPM_H__
