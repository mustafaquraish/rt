#include "integrators/ambient_occlusion.h"
#include "core/rt.h"
#include "time.h"


Colour AmbientOcclusion::Li(Ray &ray, Scene *scene, RNG& rng) {
  HitRec rec, tmp;
  if (!scene->world->hit(ray, rec))
    return Vec(0);

  if (dot(ray.d, rec.n) > 0) rec.n = -rec.n;

  Vec dir = rng.randomVectorCosineHemisphere(rec.n);
  Ray rr = Ray(rec.p, dir);
  if ( !scene->world->hit(rr, tmp) ) {
    return Vec(1);
  }
  
  return Vec(0);
}

void AmbientOcclusion::render(Scene *scene, int depth) {
  int total_samples = params.getInt("samples");
  int sx = params.getInt("width");
  int sy = params.getInt("height");

  int done = 0;
  Image im = Image(sx, sy);
  clock_t timeBegin = clock();

  #pragma omp parallel for schedule(dynamic)
  for (int i = 0; i < sx; i++) {
    // Seed RNG with some random function based on row number
    RNG rng = RNG((i * i) ^ 0xdeadbeef);

    printf("\rRendering row %d / %d ~ %f%%", done, sx, 100 * (float)done / sx);
    fflush(stdout);
    for (int j = 0; j < sy; j++) {
      Colour col = 0;
      for (int sample = 0; sample < total_samples; sample++) {
        Ray ray = scene->cam.getRay(i, j, rng);
        col += Li(ray, scene, rng);
      }
      im.set(i, j, col / total_samples);
    }
    done++;
  }

  clock_t timeEnd = clock();
  double buildTime = (double)(timeEnd - timeBegin) / CLOCKS_PER_SEC;
  printf("\n[+] Rendering completed in %.3fs\n", buildTime);

  const char *output_file = params.getStr("output");
  im.save(output_file, false);
  return;
}