#include <renderers/direct_lighting.h>
#include <renderers/path.h>
#include <core/ray.h>
#include <core/scene.h>
#include <core/object.h>
#include <core/rng.h>
#include <core/bsdf.h>

#define PATH_MAX_BOUNCES 30
#define AMBIENT_LIGHT 0.01

Colour DirectLighting::Li(Ray &r, Scene *scene, RNG& rng) {
  HitRec rec;
  
  Colour throughput = 1;

  Ray ray = Ray(r.p, r.d);
  for (int bounce = 0; bounce < PATH_MAX_BOUNCES; bounce++) {    
    if (!scene->hit(ray, rec)) break;
    
    BSDF *bsdf = rec.obj->bsdf;
    rec.wo = -ray.d;

    bsdf->initSample(rec, rng);

    if (bsdf->isEmitter()) return throughput * bsdf->emittance(rec);
    if (!bsdf->isSpecular(rec)) {
      Colour direct = throughput * SampleLight(rec, scene, rng, nullptr);
      direct += bsdf->col(rec) * AMBIENT_LIGHT;
      return direct;
    }

    throughput = throughput * bsdf->sample(rec, rng);
    ray = Ray(rec.p, rec.wi);    
  }
  return 0;
}