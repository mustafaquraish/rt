#include "core/rt.h"

using namespace std;

SCENE(Scratch) {

  Scene *scene = new Scene();
  

  Vec e = Vec(5.03f, 0.91f, -2.20f);
  Vec g = Vec(-0.21f, 1.0f, -0.34f) - e;

  // Vec e = Vec(0, 1, 2);
  // Vec g = Vec(1, 1, 0) - e;
  
  Vec up = Vec(0, 1, 0);

  scene->cam = Camera(e, g, up, 45, params);

  // scene->integrator = new Path(params);
  // scene->integrator = new DebugShader(params);
  // scene->integrator = new BaseColour(params);
  scene->integrator = new DirectLighting(params);
  
  Object *s;

  s = new TriangleMesh("assets/obj/room.obj", new Lambertian(Colour(.95)));
  // s->Translate(-2.5, -1, 0);
  // s->Scale(5);
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
