#include "integrators/path.h"
#include "core/rt.h"
#include "core/bsdf.h"
#include "time.h"

#define PATH_SAMPLES 1000
#define PATH_MAX_BOUNCES 5

Colour pptrace(Ray &r, Scene *scene) {
  HitRec rec;
  Colour L = 1;
  
  Ray ray = Ray(r.p, r.d);
  for (int bounce = 0;; bounce++) {
    
    if (bounce > PATH_MAX_BOUNCES || !scene->world->hit(ray, rec)) {
      return 0;
    }
    
    BSDF *bsdf = rec.obj->bsdf;
    if (!bsdf) { printf("BSDF IS NULL :((((\n"); }

    BSDFRec bRec = BSDFRec(ray.d, rec);
    if (bsdf->isEmitter()) {
      L = L * bsdf->emittance(bRec);
      return L;
    }

    L = L * bsdf->sample(bRec);
    ray = Ray(rec.p, bRec.wi);    
  }
  return 0;
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
      printf("\rRendering %d / %d ~ %f", done, scene->sx, done / total); fflush(stdout);
      for (int j = 0; j < scene->sy; j++) {
        Colour col = 0;
        Ray ray = scene->cam.getRay(i, j);
        for (int sample = 0; sample < PATH_SAMPLES; sample++) {
          col += pptrace(ray, scene) / PATH_SAMPLES;
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