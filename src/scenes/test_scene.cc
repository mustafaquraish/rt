#include "core/rt.h"

using namespace std;

SCENE(Test) {

  Scene *scene = new Scene();
  
  Vec e = Vec(0, 0, -15);
  Vec g = -e;
  Vec up = Vec(0, 1, 0);
  scene->cam = Camera(e, g, up, 55, params);

  // params.setWindow(0.425, 0.426, 0.605, 0.606);

  // scene->integrator = new Path(params);
  scene->integrator = new DebugShader(params);
  // scene->integrator = new BaseColour(params);
  // scene->integrator = new DirectLighting(params);
  
  Object *s;

  s = new TriangleMesh("assets/obj/teapot_lo.obj", new Lambertian(Colour(.95)));
  s->Translate(0, 0, 5);
  s->Scale(5);
  scene->add(s);


  s = new Sphere(new Emitter(Colour(.99, .7, .5)*15));
  s->Scale(1, 1, 1);
  // s->RotateX(PI/2);
  s->Translate(5.03f, 2.91f, -2.20f);
  scene->add(s);

  // scene->world = new AGGREGATE(scene->obj_list);
  scene->world = new PrimitiveList(scene->obj_list);
  
  return scene;
}
