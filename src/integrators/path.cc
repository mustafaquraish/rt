#include "integrators/path.h"
#include "core/rt.h"
#include "core/bsdf.h"
#include "time.h"

#define PATH_SAMPLES 50
#define PATH_MAX_BOUNCES 10

Colour SampleLight(HitRec& rec, Scene *scene, RNG& rng) {
  double pdf;
  HitRec tmp;
  
  // Pick a light source
  Object *light = scene->lights[ 0 ];


  // Light source point
  Vec lp = light->sample(&pdf, rng);
  // Vector from intersection pt to lightsource
  Vec wi = norm(lp - rec.p);

  Ray shadowRay = Ray(rec.p, wi);

  if (scene->hit(shadowRay, tmp) && tmp.obj == light) {
    BSDFRec bRec(rec.p, tmp, rng);
    if (dot(wi, tmp.n) > 0) return 0;

    pdf *= (tmp.t * tmp.t) / abs(dot(norm(wi), tmp.n));
    

    return rec.obj->bsdf->eval(bRec) * light->bsdf->emittance(bRec) / pdf; 
  }

  // Not visible to the lightsource.
  return Vec(0);
}

#define USE_ELS 1

Colour Path::Li(Ray &r, Scene *scene, RNG& rng) {
  HitRec rec;
  
  Colour L = 0;
  Colour throughput = 1;
  bool applyEmission = true;

  Ray ray = Ray(r.p, r.d);
  for (int bounce = 0; bounce < PATH_MAX_BOUNCES; bounce++) {
    
    if (!scene->world->hit(ray, rec)) break;
    
    BSDF *bsdf = rec.obj->bsdf;

    BSDFRec bRec = BSDFRec(ray.d, rec, rng);

    
    if (USE_ELS) {
      if (applyEmission)
        L += throughput * bsdf->emittance(bRec);
      if (!bsdf->isSpecular()) 
        L += throughput * SampleLight(rec, scene, rng);
      if (bsdf->isEmitter()) 
        break;

    } else {
      if (bsdf->isEmitter()) {
        L += throughput * bsdf->emittance(bRec);
        break;
      }
    }

    throughput = throughput * bsdf->sample(bRec);
    ray = Ray(rec.p, bRec.wi);    
    applyEmission = bsdf->isSpecular();

    // Russian roulette:
    if (bounce > 3) {
      double pr = max(throughput.r, max(throughput.g, throughput.b));
      if (rng.rand01() > pr)
        break;
      throughput *= 1.0 / pr;
    }
  }
  return L;
}

void Path::render(Scene *scene, int depth) {
  Image im = Image(scene->sx, scene->sy);
  
  clock_t timeBegin = clock();
  int done = 0;
  float total = im.sx / 100;

// #pragma omp parallel
  {
    #pragma omp parallel for schedule(dynamic)
    for (int i = 0; i < scene->sx; i++) {
      // Seed RNG with some random function based on row number
      RNG rng = RNG((i * i) ^ 0xdeadbeef);

      printf("\rRendering %d / %d ~ %f", done, scene->sx, done / total); fflush(stdout);
      for (int j = 0; j < scene->sy; j++) {
        Colour col = 0;
        for (int sample = 0; sample < PATH_SAMPLES; sample++) {
          Ray ray = scene->cam.getRay(i, j, rng);
          col += Li(ray, scene, rng) / PATH_SAMPLES;
        }
        im.set(i, j, col);
      }
      done++;
    }
  }
  clock_t timeEnd = clock();
  double buildTime = (double)(timeEnd - timeBegin) / CLOCKS_PER_SEC;
  printf("\n[+] Rendering completed in %.3fs\n", buildTime);
  cout << endl;
  im.save("output.ppm");
  return;
}