#include "integrators/path.h"
#include "core/rt.h"
#include "core/bsdf.h"
#include "time.h"

#define PATH_SAMPLES 10
#define PATH_MAX_BOUNCES 500

Colour pptrace(Ray &r, Scene *scene) {
  HitRec rec, tmp;

  Colour L = 0;
  
  for (int sample = 0; sample < PATH_SAMPLES; sample++) {
    Ray ray = Ray(r.p, r.d);
    Colour c = Colour(1.0);
    // cout << "--------------------------" << endl;
    for (int bounce = 0;; bounce++) {
      
      if (bounce > PATH_MAX_BOUNCES || !scene->world->hit(ray, rec)) {
        c = Vec(0);
        break;
      }
      
      BSDF *bsdf = rec.obj->bsdf;
      if (!bsdf) { printf("BSDF IS NULL :((((\n"); }

      BSDFRec bRec;
      bRec.n = rec.n;
      bRec.wo = ray.d;

      if (bsdf->isEmitter()) {
        c = c * bsdf->emittance(bRec);
        break;
      }

      c = c * bsdf->sample(bRec);
      ray = Ray(rec.p, bRec.wi);    
    }
    L += c;
  }
  // cout << "TOTAL: " << L << endl;
  return L;
}

void Path::render(Scene *scene, int depth) {
  Image im = Image(scene->sx, scene->sy);
  clock_t timeBegin = clock();

  int done = 0;
  for (int i = 0; i < scene->sx; i++) {
    printf("\rRendering row %d / %d ~ %f", done, scene->sx,
           100 * (float)done / scene->sx);
    fflush(stdout);
    for (int j = 0; j < scene->sy; j++) {
      // DEBUG = i == 182 && j == 145;
      Ray ray = scene->cam.getRay(i, j);
      // if (DEBUG) cout << "init ray:  p: " << ray.p << "  d: " << ray.d <<
      // endl;
      Vec col = pptrace(ray, scene);

      im.set(i, j, col);
    }
    done++;
  }
  clock_t timeEnd = clock();
  double buildTime = (double)(timeEnd - timeBegin) / CLOCKS_PER_SEC;
  printf("\n[+] Rendering completed in %.3fs\n", buildTime);
  cout << endl;
  im.save("output.ppm");
  return;
}