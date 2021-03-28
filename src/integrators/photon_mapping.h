#ifndef __PHOTON_MAPPING_H__
#define __PHOTON_MAPPING_H__

#include "core/integrator.h"
#include "util/params.h"
#include "core/primitive.h"
#include "core/pointkdtree.h"

struct PhotonMapping : Integrator {
  PhotonMapping(RenderParams params) : Integrator(params) {
    if (params.exists("numPhotons"))
      numPhotons = params.get<int>("numPhotons");
    if (params.exists("photonRange"))
      numPhotons = params.get<double>("photonRange");
    
    // gammaCorrect = false;
  };

  void sendPhoton(Scene *scene, RNG& rng, int depth);
  virtual void render(Scene *scene);
  Colour Li(Ray& r, Scene *scene, RNG& rng);

  int numPhotons = 1000000;
  double photonRange = .2; // ??
  PointKDTree *photonMap;
};

#endif // __PHOTON_MAPPING_H__
