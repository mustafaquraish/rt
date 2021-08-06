#include <integrators/base_colour.h>

#define PATH_MAX_BOUNCES 30

Colour BaseColour::Li(Ray &r, Scene *scene, RNG& rng) {
  HitRec rec;
  Colour throughput = 1;

  Ray ray = Ray(r.p, r.d);
  for (int bounce = 0; bounce < PATH_MAX_BOUNCES; bounce++) {    
    if (!scene->hit(ray, rec)) break;
    
    BSDF *bsdf = rec.obj->bsdf;
    rec.wo = -ray.d;

    throughput = throughput * bsdf->col(rec);

    if (!bsdf->isSpecular()) return throughput;

    bsdf->sample(rec, rng);
    ray = Ray(rec.p, rec.wi);    
  }
  return 0;
}