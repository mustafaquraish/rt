#include "integrators/base_colour.h"

#define PATH_MAX_BOUNCES 30

bool _DBG = false;
using namespace std;

Colour BaseColour::Li(Ray &r, Scene *scene, RNG& rng) {
  HitRec rec;
  Colour throughput = 1;

  Ray ray = Ray(r.p, r.d);
  for (int bounce = 0; bounce < PATH_MAX_BOUNCES; bounce++) {    
    if (!scene->world->hit(ray, rec)) break;
    
    BSDF *bsdf = rec.obj->bsdf;
    rec.wo = -ray.d;

    throughput = throughput * bsdf->col(rec);
    if (_DBG) cout << "Throughput: " << throughput << endl;

    if (!bsdf->isSpecular()) return throughput;

    bsdf->sample(rec, rng);
    ray = Ray(rec.p, rec.wi);    
  }
  return 0;
}

void BaseColour::render(Scene *scene, int depth) {
  int total_samples = params.getInt("samples");
  int sx = params.getInt("width");
  int sy = params.getInt("height");
  
  Image im = Image(sx, sy);

  int done = 0;
  float total = sx / 100;
  
  clock_t timeBegin = clock();


  for (int i = 0; i < sx; i++) {
    // Seed RNG with some random function based on row number
    RNG rng = RNG(1);

    // printf("\rRendering %d / %d ~ %.2f", done, sx, done/total); fflush(stdout);
    for (int j = 0; j < sy; j++) {
      Colour col = 0;
      for (int sample = 0; sample < total_samples; sample++) {
        Ray ray = scene->cam.getRay(i, j);
        _DBG = (i == 14 && j == 33);
        col += Li(ray, scene, rng) / total_samples;
      if (_DBG) cout << "Col: " << col << endl;
        // if (i == 14 && j == 33) col = Colour(0, 1, 0);
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