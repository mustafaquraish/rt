#include "integrators/ambient_occlusion.h"

Colour AmbientOcclusion::Li(Ray &ray, Scene *scene, RNG& rng) {
  HitRec rec, tmp;
  if (!scene->world->hit(ray, rec))
    return Vec(0);

  if (dot(ray.d, rec.n) > 0) rec.n = -rec.n;

  Vec dir = rng.randomVectorCosineHemisphere(rec.n);
  Ray rr = Ray(rec.p, dir);
  if ( !scene->world->hit(rr, tmp) ) {
    return Vec(1);
  }
  
  return Vec(0);
}