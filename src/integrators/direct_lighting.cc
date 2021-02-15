#include "integrators/direct_lighting.h"
#include "core/rt.h"
#include "time.h"
#include "core/bsdf.h"

#define PATH_MAX_BOUNCES 10
#define AMBIENT_LIGHT 0.01

Vec cmpWiseMax(const Vec& a, const Vec& b) {
  return Vec(
    max(a.x, b.x),
    max(a.y, b.y),
    max(a.z, b.z)
  );
}

Colour DirectLighting::SampleLight(HitRec& rec, Scene *scene, RNG& rng) {
  double pdf;
  HitRec tmp;
  
  // Pick a light source
  Object *light = scene->lights[ rng.randint() % scene->lights.size() ];


  // Light source point
  Vec lp = light->sample(&pdf, rng);
  // Vector from intersection pt to lightsource
  Vec wi = norm(lp - rec.p);
  rec.wi = wi;

  Vec contrib = AMBIENT_LIGHT;

  Ray shadowRay = Ray(rec.p, wi);
  if (scene->hit(shadowRay, tmp) && tmp.obj == light) {
      // Light ray from bad direction
      if (dot(wi, tmp.n) < 0 || dot(wi, rec.n) > 0) {
      
      tmp.wo = -shadowRay.d;

      pdf *= (tmp.t * tmp.t);
      pdf /= dot(norm(wi), rec.n) * -dot(norm(wi), tmp.n);
      pdf /= scene->lights.size();
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
    if (!scene->world->hit(ray, rec)) break;
    
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

void DirectLighting::render(Scene *scene, int depth) {
  int total_samples = params.getInt("samples");
  int sx = params.getInt("width");
  int sy = params.getInt("height");
  
  Image im = Image(sx, sy);

  int done = 0;
  float total = sx / 100;
  
  clock_t timeBegin = clock();


  #pragma omp parallel for schedule(dynamic)
  for (int i = 0; i < sx; i++) {
    // Seed RNG with some random function based on row number
    RNG rng = RNG((i * i) ^ 0xdeadbeef);

    printf("\rRendering %d / %d ~ %.2f", done, sx, done/total); fflush(stdout);
    for (int j = 0; j < sy; j++) {
      Colour col = 0;
      for (int sample = 0; sample < total_samples; sample++) {
        Ray ray = scene->cam.getRay(i, j, rng);
        col += Li(ray, scene, rng) / total_samples;
      }
      im.set(i, j, col);
    }
    done++;
  }

  clock_t timeEnd = clock();
  double buildTime = (double)(timeEnd - timeBegin) / CLOCKS_PER_SEC;
  printf("\n[+] Rendering completed in %.3fs\n", buildTime);
  cout << endl;

  const char *output_file = params.getStr("output");
  im.save(output_file);
  // im.saveHDR(output_file);
  return;
}