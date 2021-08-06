#include <integrators/sppm.h>
#include <util/progress.h>
#include <util/timer.h>

void SPPM::addVisiblePoint(const Ray& r, Scene *scene, RNG& rng, int pixIdx) {
  
  Colour throughput = 1;
  HitRec rec;
  Ray ray = r;
  
  for (int i = 0; i < depth; i++) {
    if (!scene->world->hit(ray, rec)) break;

    BSDF *bsdf = rec.obj->bsdf;
    rec.wo = -ray.d;

    if (bsdf->isEmitter()) {
      // We hit a light directly. No need to add it to the photon map, we know
      // the final colour already :)
      photons[pixIdx].acc = throughput * bsdf->emittance(rec);
      photons[pixIdx].M = 1;
      return;
    }

    if (!bsdf->isSpecular()) {
      photons[pixIdx] = Photon(rec.p, -ray.d, throughput);
      photonMap->emplacePoint(rec.p, &photons[pixIdx]);
      return;
    }
    throughput *= bsdf->sample(rec, rng);
    ray = Ray(rec.p, rec.wi);
  }
  // Need to give default value to params to avoid junk. Since this is never
  // added to the photonmap, the colour for this pixel will not be changed in 
  // this iteration.
  photons[pixIdx] = Photon(0,0,0);
}

void SPPM::sendPhoton(Scene *scene, RNG &rng) {
  if (scene->lights.size() == 0) return; 

  float pdf;

  // Pick a light source, and pick a point on it
  Object *light = scene->lights[ rng.randint() % scene->lights.size() ];
  Vec3 lp = light->sample(&pdf, rng);

  // Pick a direction from the light source
  // TODO: Find the correct normal and don't assume it's (0, -1, 0)
  Vec3 lsnorm = Vec3(0,-1, 0);
  Vec3 dir = rng.randomVectorCosineHemisphere(lsnorm);

  // TODO: passing dummy rec for now, fix this if needed.
  HitRec rec;
  Colour throughput = light->bsdf->emittance(rec);
  
  Ray ray = Ray(lp, dir);
  for (int i = 0; i < depth; i++) {
    if (!scene->world->hit(ray, rec)) break;

    BSDF *bsdf = rec.obj->bsdf;
    rec.wo = -ray.d;

    std::vector<PhotonNode> nearPoints;
    photonMap->getRange(rec.p, photonRange, nearPoints);
    if (nearPoints.size() > 0) {
      HitRec bRec = rec;
      bRec.wi = rec.wo;
      for (auto pt : nearPoints) {
        Photon *p = pt.photon;
        bRec.wo = p->dir;
        bRec.p = rec.p;
        p->acc = p->acc + (Colour)(p->col * bsdf->eval(bRec) * throughput);
        p->M++;
      }
    }

    throughput *= bsdf->sample(rec, rng);
    ray = Ray(rec.p, rec.wi);
  }
}

void SPPM::render(Scene *scene) {

  // Create Image
  int sx = params.get<int>("width");
  int sy = params.get<int>("height");
  Image im = Image(sx, sy);
  photons.reserve(sx * sy);

  for (int iter = 0; iter < numIterations; iter++) {
    photons.clear();

    // Add visible points to the photon map
    photonMap = new PointKDTree<PhotonNode>(20);
    for (int y = 0; y < sy; y++) {
      RNG rng;
      for (int x = 0; x < sx; x++) {
        Colour col = 0;
        Ray ray = scene->cam.getRay(x, y, rng);
        addVisiblePoint(ray, scene, rng, x + y*sx); 
      }
    }

    int updateInc = numPhotonsPerIter / 100; 
    std::string tx = "Iteration: " + std::to_string(iter) + "/" + std::to_string(numIterations) + ", Sending Photons"; 
    ProgressBar progress = ProgressBar(numPhotonsPerIter, tx, updateInc);
    
    #pragma omp parallel for schedule(dynamic, 128)
    for (size_t p = 0; p < numPhotonsPerIter; p++) {
      RNG rng;
      sendPhoton(scene, rng);
      progress.update();
    }

    for (int y = 0; y < sy; y++) {
      for (int x = 0; x < sx; x++) {
        Photon *p = &photons[x + y*sx];
        if (p->M) im.splat(x, y, p->acc / p->M);
      }
    }

    delete photonMap;
    char *output = params.get<char *>("output");
    im.save(output, gammaCorrect, exposure / (iter+1));
  }

  return;
}