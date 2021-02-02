#include "integrators/ambient_occlusion.h"
#include "core/rt.h"
#include "time.h"

#define AA_SAMPLES 500
#define MAX_AA_DIST INFINITY

Colour otrace(Ray &ray, Scene *scene) {
  HitRec rec, tmp;
  if (!scene->world->hit(ray, rec))
    return Vec(0);

  if (dot(ray.d, rec.n) > 0) rec.n = -rec.n;

  double sum = 0;
  for (int i = 0; i < AA_SAMPLES; i++) {
    Vec dir = randomVectorCosineHemisphere(rec.n);
    Ray rr = Ray(rec.p, dir, MAX_AA_DIST);
    if ( !scene->world->hit(rr, tmp) ) {
      sum += 1.0;
    }
  }
  
  return sum / AA_SAMPLES;
}

void AmbientOcclusion::render(Scene *scene, int depth) {
  Image im = Image(scene->sx, scene->sy);
  clock_t timeBegin = clock();

  int done = 0;
  #pragma omp parallel for
  for (int i = 0; i < scene->sx; i++) {
    printf("\rRendering row %d / %d ~ %f", done, scene->sx,
           100 * (float)done / scene->sx);
    fflush(stdout);
    for (int j = 0; j < scene->sy; j++) {
      // DEBUG = i == 182 && j == 145;
      Ray ray = scene->cam.getRay(i, j);
      // if (DEBUG) cout << "init ray:  p: " << ray.p << "  d: " << ray.d <<
      // endl;
      Vec col = otrace(ray, scene);

      im.set(i, j, col);
    }
    #pragma omp atomic
    done++;
  }
  clock_t timeEnd = clock();
  double buildTime = (double)(timeEnd - timeBegin) / CLOCKS_PER_SEC;
  printf("\n[+] Rendering completed in %.3fs\n", buildTime);
  cout << endl;
  im.save("output.ppm");
  return;
}