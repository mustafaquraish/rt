#pragma once

#include <core/renderer.h>
#include <util/params.h>
#include <core/primitive.h>
#include <core/pointkdtree.h>



struct PhotonMapping : Renderer {
  
  PhotonMapping(RenderParams params) : Renderer(params) {
    if (params.exists("numPhotons"))
      numPhotons = params.get<int>("numPhotons");
    if (params.exists("photonRange"))
      photonRange = params.get<float>("photonRange");
    
    // gammaCorrect = false;
  };

  void sendPhoton(Scene *scene, RNG& rng, int depth);
  virtual void render(Scene *scene);
  Colour Li(Ray& r, Scene *scene, RNG& rng);

  struct Photon {
    Vec3 pos;
    Vec3 dir;
    Vec3 col;
    Photon(Vec3 pos, Vec3 dir, Vec3 col) : pos(pos), dir(dir), col(col) {};
  };

  int numPhotons = 1000000;
  float photonRange = .2; // ??
  PointKDTree<Photon> *photonMap;
};

