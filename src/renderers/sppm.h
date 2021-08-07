#pragma once

#include <core/renderer.h>
#include <util/params.h>
#include <core/primitive.h>
#include <core/pointkdtree.h>

#include <atomic>

struct SPPM : Renderer {
  SPPM(RenderParams params) : Renderer(params) {
    if (params.exists("numPhotons"))
      numPhotonsPerIter = params.get<int>("numPhotons");
    if (params.exists("numIterations"))
      numIterations = params.get<int>("numIterations");
    if (params.exists("photonRange"))
      photonRange = params.get<float>("photonRange");
  };

  void addVisiblePoint(const Ray& ray, Scene *scene, RNG& rng, int pixelIdx);
  void sendPhoton(Scene *scene, RNG &rng);
  virtual void render(Scene *scene);

  struct Photon {
    Vec3 pos, dir;
    Colour col;
    Colour acc = Colour(0);
    int M = 0;
    Photon(Vec3 pos, Vec3 dir, Vec3 col) : pos(pos), dir(dir), col(col), M(0) {};
  };

  struct PhotonNode {
    Vec3 pos;
    Photon *photon;
    PhotonNode() {};
    PhotonNode(Vec3 pos, Photon *photon): pos(pos), photon(photon) {};
  };

  const int depth = 10;
  int numIterations = 10;
  int numPhotonsPerIter = 10000;
  float photonRange = .2; // ??
  std::vector<Photon> photons;
  PointKDTree<PhotonNode> *photonMap;
};

