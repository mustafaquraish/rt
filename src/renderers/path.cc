#include <renderers/path.h>

#define PATH_MAX_BOUNCES 30
#define MIN_PDF_LIGHT 0.1

using namespace std;

Colour Path::SampleLight(HitRec& rec, Scene *scene, RNG& rng) {
  float pdf;
  HitRec tmp;
  
  if (scene->lights.size() == 0) return 0; 

  // Pick a light source
  // Object *light = scene->lights[ 0 ];
  Object *light = scene->lights[ rng.randint() % scene->lights.size() ];


  // Light source point
  Vec3 lp = light->sample(&pdf, rng);
  
  // Vector from intersection pt to lightsource
  Vec3 wi = normalized(lp - rec.p);
  rec.wi = wi;

  Ray shadowRay = Ray(rec.p, wi);

  if (scene->hit(shadowRay, tmp) && tmp.obj == light) {
    // Light ray from bad direction
    if (dot(wi, tmp.n) > 0 || dot(wi, rec.n) < 0) return 0;
    
    tmp.wo = -shadowRay.d;

    pdf *= (tmp.t * tmp.t);
    pdf /= fabs(dot(normalized(wi), rec.n) * -dot(normalized(wi), tmp.n));
    pdf /= scene->lights.size();

    if (pdf < MIN_PDF_LIGHT) pdf = MIN_PDF_LIGHT;

    return rec.obj->bsdf->eval(rec) * light->bsdf->emittance(tmp) / pdf; 
  }

  // Not visible to the lightsource.
  return Colour(0);
}

Colour Path::Li(Ray &r, Scene *scene, RNG& rng) {
  HitRec rec;
  
  Colour L = 0;
  Colour throughput = 1;
  bool applyEmission = true;

  Ray ray = Ray(r.p, r.d);
  for (int bounce = 0; bounce < PATH_MAX_BOUNCES; bounce++) {
    if (!scene->hit(ray, rec)) break;

    BSDF *bsdf = rec.obj->bsdf;
    rec.wo = -ray.d;

#if 1
    if (applyEmission) L += throughput * bsdf->emittance(rec);
    if (bsdf->isEmitter()) break;
    if (!bsdf->isSpecular()) L += throughput * SampleLight(rec, scene, rng);
#else
    if (bsdf->isEmitter()) return throughput * bsdf->emittance(rec);
#endif

    throughput *= bsdf->sample(rec, rng);
    applyEmission = bsdf->isSpecular();  // Only apply direct emission if spec.

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