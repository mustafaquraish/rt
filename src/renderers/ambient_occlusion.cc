#include <renderers/ambient_occlusion.h>

Colour AmbientOcclusion::Li(Ray &ray, Scene *scene, RNG& rng) {
  HitRec rec, tmp;
  if (!scene->world->hit(ray, rec))
    return Vec3(0);

  Colour col = rec.obj->bsdf->col(rec);

  if (dot(ray.d, rec.n) > 0) rec.n = -rec.n;

  Vec3 dir = rng.randomVectorCosineHemisphere(rec.n);
  Ray rr = Ray(rec.p, dir);
  if ( !scene->world->hit(rr, tmp) ) {
    return col;
  }
  
  return Vec3(0);
}