#include "core/rt.h"
#include "integrators/debug_shader.h"
#include "time.h"

int DEBUG = 0;

void DebugShader::render(Scene *scene, int depth) {
  Image im = Image(scene->sx, scene->sy);
  clock_t timeBegin = clock();

  int done = 0;
  // #pragma omp parallel for schedule(dynamic, 64)
  for (int i = 0; i < scene->sx; i++) {
    printf("\rRendering row %d / %d ~ %f", done, scene->sx, 100 * (float)done/scene->sx);
    fflush(stdout);
    for (int j = 0; j < scene->sy; j++) {
      // DEBUG = i == 182 && j == 145;
      Ray ray = scene->cam.getRay(i, j);
      if (DEBUG) cout << "init ray:  p: " << ray.p << "  d: " << ray.d << endl; 
      Vec col = Vec(0);

      HitRec rec;
      if (scene->world->hit(ray, rec)) {

        /****************** NORMALS ****************/
        if (type == DEBUG_NORMALS) col = (rec.n + 1) / 2;

        /****************** DISTANCE ***************/
        if (type == DEBUG_DEPTH) col = 1 / rec.t;

        /***************** MATERIAL COLOUR *********/
        if (type == DEBUG_COLOUR) {
          Material *mat = rec.obj->getMaterial(rec); 
          col = (mat) ? mat->get() : Vec();
        }
      }

      im.set(i, j, col);
    }
    // #pragma omp atomic
    done++;
  }
  clock_t timeEnd = clock();
  double buildTime = (double)(timeEnd - timeBegin) / CLOCKS_PER_SEC;
  printf("\n[+] Rendering completed in %.3fs\n", buildTime);
  cout << endl;
  im.save("output.ppm");
  return;
}