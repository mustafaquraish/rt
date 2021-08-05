#include "integrators/photon_mapping.h"
#include "util/progress.h"

void PhotonMapping::sendPhoton(Scene *scene, RNG &rng, int depth) {
  if (scene->lights.size() == 0) return; 

  double pdf;

  // Pick a light source, and pick a point on it
  Object *light = scene->lights[ rng.randint() % scene->lights.size() ];
  Vec lp = light->sample(&pdf, rng);

  // Pick a direction from the light source
  // TODO: Find the correct normal and don't assume it's (0, -1, 0)
  Vec lsnorm = Vec(0,-1, 0);
  Vec dir = rng.randomVectorCosineHemisphere(lsnorm);

  
  // TODO: passing dummy rec for now, fix this if needed.
  HitRec rec;
  Colour throughput = light->bsdf->emittance(rec);
  
  Ray ray = Ray(lp, dir);
  for (int i = 0; i < depth; i++) {
    if (!scene->world->hit(ray, rec)) break;

    BSDF *bsdf = rec.obj->bsdf;
    rec.wo = -ray.d;

    if (!bsdf->isSpecular()) {
      photonMap->addPoint(Photon(rec.p, -ray.d, throughput));
    }

    throughput *= bsdf->sample(rec, rng);



    ray = Ray(rec.p, rec.wi);
  }
}

void PhotonMapping::render(Scene *scene) {
  photonMap = new PointKDTree<Photon>(20);

  int updateInc = numPhotons / 100; 
  ProgressBar progress = ProgressBar(numPhotons, "Shooting Photons", updateInc);

  RNG rng;
  for (int i = 0; i < numPhotons; i++) {
    progress.update();
    sendPhoton(scene, rng, 10);
  }

  printf("\nDone. Now calling Integrator::render()\n");
  Integrator::render(scene);
  delete photonMap;
}

Colour PhotonMapping::Li(Ray& r, Scene *scene, RNG& rng) {
  HitRec rec;
  Colour throughput = 1;

  Ray ray = Ray(r.p, r.d);
  for (int bounce = 0; bounce < 10; bounce++) {    
    if (!scene->hit(ray, rec)) break;
    
    BSDF *bsdf = rec.obj->bsdf;
    rec.wo = -ray.d;

    if (bsdf->isEmitter()) return throughput * bsdf->emittance(rec);

    if (!bsdf->isSpecular()) {
      std::vector<Photon> photons;
      photonMap->getRange(rec.p, photonRange, photons);

      Colour c = 0;
      for (auto p : photons) {
        rec.wi = p.dir;
        c += p.col * bsdf->eval(rec);
      }
      // for (auto p : photons) c += p.col * (1-length(rec.p - p.pos)/photonRange);
      
      if (photons.size() > 0) c /= photons.size();
      return throughput * c;
    }

    throughput *= bsdf->sample(rec, rng);
    ray = Ray(rec.p, rec.wi);    
  }
  return 0;
}
