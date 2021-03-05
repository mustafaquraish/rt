#include "core/rt.h"

SCENE(SDF) {

  Scene *scene = new Scene();
  
  Vec e = Vec(0, 0, -15);
  Vec g = -e;
  Vec up = Vec(0, 1, 0);
  scene->cam = Camera(e, g, up, 50, params);

  // params.setWindow(0.2, 0.4, 0.4, 0.6);

  // scene->integrator = new DebugShader(params);
  scene->integrator = new AmbientOcclusion(params);
  // scene->integrator = new BaseColour(params);


  double frameRatio = params.get<double>("frameRatio");

  Object *s;

  // s = new SphereSDF(new Lambertian(1));
  // scene->add(s);

  // double R = lerp(frameRatio, 0.0, 4.0);
  // s = new InfiniteSphereSDF(new Lambertian(1));
  // s->Translate(R, R, 0);
  // scene->add(s);

  s = new MandelBulbSDF(new Lambertian(1));
  s->addTextureMap(new SDFTexture(Colour(1)));
  s->RotateX(PI/2);
  s->Scale(5);
  scene->add(s);

  scene->world = new PrimitiveList(scene->obj_list);
  
  return scene;
}
