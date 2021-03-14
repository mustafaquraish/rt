#include "core/rt.h"

using namespace std;

SCENE(Env) {
  double frameRatio = params.get<double>("frameRatio");
  Scene *scene = new Scene();
  
  Vec e = Vec(0, 0, -40);
  Vec g = -e;
  Vec up = Vec(0, 1, 0);
  scene->cam = Camera(e, g, up, 35, params);

  // params.set("exposure", 0.8);

  scene->integrator = new Path(params);
  // scene->integrator = new DebugShader(params);
  //  scene->integrator = new DirectLighting(params);
  // scene->integrator = new BaseColour(params);
  
  Object *s;

  double R = lerp(frameRatio, 0.0, TAU);
  s = new EnvironmentMap("assets/hdri/snow.ppm");
  s->RotateY(R);
  scene->addEnvMap(s);

  // scene->addEnvMap("assets/tex/water-norm.ppm");
  // scene->addEnvMap(Colour(0.7, 1, 0.7));
  // scene->addEnvMap("assets/hdri/ss.ppm");
  // scene->addEnvMap("assets/hdri/snow.ppm");

  s = new Sphere(new Transmissive(1.47, Colour(0.7)));
  s = new Sphere(new Mirror(Colour(0.9)));
  s->Scale(4);
  s->Translate(-4, 0, 0);
  // scene->add(s);

  s = new Sphere(new Transmissive(1.47, Colour(0.7)));
  s->Scale(4);
  s->Translate(4, 0, 0);
  // scene->add(s);

  s = new TriangleMesh("assets/obj/dragon.obj", new Mirror(0.5));
  s->Scale(2.5);
  s->Translate(0,-4,-5);
  scene->add(s);

  s = new Plane(new Lambertian(Colour(0.5)));
  s->addTextureMap(new CheckerTexture(0.01));
  s->Scale(100);
  s->RotateZ(PI / 4);
  s->RotateX(PI / 2);
  s->Translate(0, -4, 5);
  scene->add(s);


  // scene->world = new AGGREGATE(scene->obj_list);
  scene->world = new PrimitiveList(scene->obj_list);
  
  return scene;
}
