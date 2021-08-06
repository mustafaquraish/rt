#include <integrators/debug_shader.h>

Colour DebugShader::Li(Ray &ray, Scene *scene, RNG& rng) {
  HitRec rec;
  Colour col;
  if (scene->world->hit(ray, rec)) {
    if (type == DEBUG_NORMALS) col = inverseLerp(rec.n, -1, 1);
    if (type == DEBUG_DEPTH) col = 1 / rec.t;
    if (type == DEBUG_COLOUR) col = rec.obj->bsdf->col(rec);
  }
  return col;
}