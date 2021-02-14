#include "core/rt.h"

SCENE(AjaxDragon) {

  Scene *scene = new Scene();
  
  Vec e = Vec(0, 0, -15);
  Vec g = -e;
  Vec up = Vec(0, 1, 0);
  scene->cam = Camera(e, g, up, -4, params);

  // scene->integrator = new DebugShader(params);
  scene->integrator = new AmbientOcclusion(params);

  Object *s;

  s = new TriangleMesh("assets/obj/ajax.obj", new Lambertian(Colour(1, 0, 0)));
  s->RotateY(-PI/4 - PI/2);
  s->Scale(0.25, 0.25, 0.25);
  s->RotateY(-PI/8);
  s->Translate(-6, -4, -7);
  scene->add(s);

  s = new TriangleMesh("assets/obj/dragon.obj", new Lambertian(Colour(1, 0, 0)));
  s->RotateY(-PI/4);
  s->Scale(2, 2, 2);
  // s->RotateY(-PI/8);
  s->Translate(5, -4, 0);
  scene->add(s);

  s = new Plane(new Lambertian(Colour(0.55, 0.8, 0.75)));
  s->Scale(20, 20, 20);
  s->RotateZ(PI / 4);
  s->RotateX(PI / 2);
  s->Translate(0, -4, 5);
  scene->add(s);

  scene->world = new AGGREGATE(scene->obj_list);
  
  return scene;
}