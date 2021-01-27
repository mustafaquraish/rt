#include "core/rt.h"
#include "integrators/debug_shader.h"

int DEBUG = 0;

void DebugShader::render(Scene *scene, int depth) {
  Image im = Image(scene->sx, scene->sy);
  for (int i = 0; i < scene->sx; i++) {
    printf("\rRendering row %d / %d ~ %f", i, scene->sx, 100 * (float)i/scene->sx);
    fflush(stdout);
    for (int j = 0; j < scene->sy; j++) {
      // DEBUG = i == 182 && j == 145;
      Ray ray = scene->cam.getRay(i, j);
      if (DEBUG) cout << "init ray:  p: " << ray.p << "  d: " << ray.d << endl; 
      Vec col = Vec(0);

      HitRec rec;
      if (scene->world->hit(ray, rec)) {

        /****************** NORMALS ****************/
        col = (rec.n + 1) / 2;

        /****************** DISTANCE ***************/
        // col = 1 / rec.t1;

        /***************** MATERIAL COLOUR *********/
        // Material *mat = rec.obj->getMaterial(rec); col = (mat) ? mat->get() : Vec();


      }

      im.set(i, j, col);
    }
  }
  cout << endl;
  im.save("output.ppm");
  return;
}