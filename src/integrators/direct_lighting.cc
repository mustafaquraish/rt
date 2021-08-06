#include <integrators/direct_lighting.h>

#define PATH_MAX_BOUNCES 30
#define AMBIENT_LIGHT 0.01

using namespace std;


Colour DirectLighting::SampleLight(HitRec& rec, Scene *scene, RNG& rng) {
  double pdf;
  HitRec tmp;
  
  // Pick a light source
  Object *light = scene->lights[ rng.randint() % scene->lights.size() ];


  // Light source point
  Vec3 lp = light->sample(&pdf, rng);
  // Vector from intersection pt to lightsource
  Vec3 wi = norm(lp - rec.p);
  rec.wi = wi;

  Vec3 contrib = AMBIENT_LIGHT;

  Ray shadowRay = Ray(rec.p, wi);
  if (scene->hit(shadowRay, tmp) && tmp.obj == light) {
    // Light ray from bad direction
    if (dot(wi, tmp.n) < 0 || dot(wi, rec.n) > 0) {
      
      tmp.wo = -shadowRay.d;

      pdf *= (tmp.t * tmp.t);
      pdf /= fabs(dot(norm(wi), rec.n) * -dot(norm(wi), tmp.n));
      pdf /= scene->lights.size();

      if (pdf < 1) pdf = 1;

      contrib = cmpWiseMax(contrib, light->bsdf->emittance(tmp) / pdf);
    }
  }

  // Not visible to the lightsource.
  return rec.obj->bsdf->eval(rec) * contrib;
}

Colour DirectLighting::Li(Ray &r, Scene *scene, RNG& rng) {
  HitRec rec;
  
  Colour throughput = 1;

  Ray ray = Ray(r.p, r.d);
  for (int bounce = 0; bounce < PATH_MAX_BOUNCES; bounce++) {    
    if (!scene->hit(ray, rec)) break;
    
    BSDF *bsdf = rec.obj->bsdf;
    rec.wo = -ray.d;

    if (bsdf->isEmitter()) 
      return throughput * bsdf->emittance(rec);
    if (!bsdf->isSpecular()) 
      return throughput * SampleLight(rec, scene, rng);

    throughput = throughput * bsdf->sample(rec, rng);
    ray = Ray(rec.p, rec.wi);    
  }
  return 0;
}