#include <renderers/path.h>
#include <core/ray.h>
#include <core/scene.h>
#include <core/object.h>
#include <core/rng.h>
#include <core/bsdf.h>

#define PATH_MAX_BOUNCES 30
#define MIN_PDF_LIGHT 0.1
#define USE_LIGHT_SAMPLING 1

using namespace std;

__attribute__((flatten))
Colour SampleLight(HitRec& rec, Scene *scene, RNG& rng, Object** sampledLight) {
  if (scene->lights.size() == 0) return 0;

  float pdf;
  HitRec tmp;
  
  Object *light = scene->lights[ rng.randint() % scene->lights.size() ];
  if (sampledLight) *sampledLight = light;
  Vec3 light_point = light->sample(&pdf, rng);
  
  // Vector from intersection pt to lightsource
  Vec3 wi = normalized(light_point - rec.p);
  rec.wi = wi;

  Ray shadowRay = Ray(rec.p, wi);

  float wi_dot_n = dot(wi, rec.n);
  if (wi_dot_n < 0) return 0;

  if (scene->hit(shadowRay, tmp) && tmp.obj == light) {
    float wi_dot_ls_n = dot(wi, tmp.n);
    // Light ray from bad direction
    if (wi_dot_ls_n > 0) return 0;
    
    tmp.wo = -shadowRay.d;

    pdf *= (tmp.t * tmp.t);
    pdf /= wi_dot_n * -wi_dot_ls_n;
    pdf /= scene->lights.size();

    if (pdf < MIN_PDF_LIGHT) pdf = MIN_PDF_LIGHT;

    return rec.obj->bsdf->eval(rec) * light->bsdf->emittance(tmp) / pdf; 
  }

  // Not visible to the lightsource.
  return Colour(0);
}

__attribute__((flatten))
Colour Path::Li(Ray &r, Scene *scene, RNG& rng) {
  HitRec rec;
  
  Colour L = 0;
  Colour throughput = 1;
  bool applyEmission = true;
  Object *sampledLight = nullptr;

  Ray ray = Ray(r.p, r.d);
  for (int bounce = 0; bounce < PATH_MAX_BOUNCES; bounce++) {
    if (!scene->hit(ray, rec)) break;

    BSDF *bsdf = rec.obj->bsdf;
    rec.wo = -ray.d;

    bsdf->initSample(rec, rng);

#if USE_LIGHT_SAMPLING
    if (rec.obj != sampledLight) L += throughput * bsdf->emittance(rec);
    if (bsdf->isEmitter()) break;
    sampledLight = nullptr;
    if (!bsdf->isSpecular(rec)) L += throughput * SampleLight(rec, scene, rng, &sampledLight);
#else
    if (bsdf->isEmitter()) return throughput * bsdf->emittance(rec);
#endif

    throughput *= bsdf->sample(rec, rng);

    // Russian roulette:
    if (bounce > 3) {
      float pr = max(throughput);
      if (rng.rand01() > pr) break;
      throughput *= 1.0 / pr;
    }

    ray = Ray(rec.p, rec.wi);    
  }
  return L;
}

#undef MIN_PDF_LIGHT