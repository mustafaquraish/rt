#include "core/rt.h"
#include "integrators/debug_shader.h"
#include "time.h"

int DEBUG = 0;

void DebugShader::render(Scene *scene, int depth) {
  int sx = params.getInt("width");
  int sy = params.getInt("height");

  int done = 0;
  Image im = Image(sx, sy);
  clock_t timeBegin = clock();

  // #pragma omp parallel for schedule(dynamic, 64)
  for (int i = 0; i < sx; i++) {
    
    printf("\rRendering row %d / %d ~ %.2f%%", done, sx, 100 * (float)done/sx);
    fflush(stdout);

    for (int j = 0; j < sy; j++) {
      // DEBUG = i == 182 && j == 145;
      Ray ray = scene->cam.getRay(i, j);
      Vec col = Vec(0);

      HitRec rec;
      if (scene->world->hit(ray, rec)) {

        /****************** NORMALS ****************/
        if (type == DEBUG_NORMALS) col = map(rec.n, -1, 1, 0, 1);

        /****************** DISTANCE ***************/
        if (type == DEBUG_DEPTH) col = 1 / rec.t;

        /****************** DISTANCE ***************/
        if (type == DEBUG_COLOUR) col = rec.obj->bsdf->col(rec);

      }

      im.set(i, j, col);
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