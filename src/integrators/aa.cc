#include "integrators/aa.h"
#include "core/rt.h"
#include "time.h"

#define AA_SAMPLES 100
#define MAX_AA_DIST 1.5

Vec cosWeightedSample(const Vec &n) {
  // Random sample on hemisphere with cosine-weighted distribution
  double r = sqrt(rand01());
  double theta = 2 * PI * rand01();

  double x = r * cos(theta);
  double y = r * sin(theta);
  double z = sqrt(1.0 - (x * x) - (y * y));
  Vec d = Vec(x, y, z);

  // Transform from hemisphere to normal-space
  return getRotationMatrix(n) * d;
}

Colour otrace(Ray &ray, Scene *scene) {
  HitRec rec;
  if (!scene->world->hit(ray, rec))
    return Vec(0);
  Colour col = Vec(1);

  // if (dot(ray.d, rec.n) > 0) rec.n = -rec.n;

  std::vector<double> vals;
  for (int i = 0; i < AA_SAMPLES; i++) {
    // Ray rr = Ray(rec.p, randomVectorHemisphere(rec.n), MAX_AA_DIST);
    Ray rr = Ray(rec.p, cosWeightedSample(rec.n), MAX_AA_DIST);
    if (scene->world->hit(rr, rec))
      vals.push_back(rec.t / MAX_AA_DIST);
    else
      vals.push_back(1);
  }

  if (vals.size() > 0) {
    double sum = 0;
    for (double v : vals) {
      sum += powf(v, 0.5);
    }
    col = Vec(sum / AA_SAMPLES);
  }

  return col;
}

void AmbientOcclusion::render(Scene *scene, int depth) {
  Image im = Image(scene->sx, scene->sy);
  clock_t timeBegin = clock();

  int done = 0;
#pragma omp parallel for schedule(dynamic, 64)
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