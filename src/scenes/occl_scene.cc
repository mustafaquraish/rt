#include "core/rt.h"

SCENE(Occlusion) {

  Scene *scene = new Scene();
  
  Vec e = Vec(0, 0, -15);
  Vec g = -e;
  Vec up = Vec(0, 1, 0);
  scene->cam = Camera(e, g, up, -4, params);

  scene->integrator = new AmbientOcclusion(params);

  Object *s;

  s = new TriangleMesh("assets/obj/ev3.obj", new Lambertian(Colour(1, 0, 0)));
  // s->RotateY(PI);
  s->Scale(0.4, 0.4, 0.4);
  s->RotateX(PI);
  s->Translate(-1, -3.5, 3.);
  scene->add(s);

  s = new TriangleMesh("assets/obj/pinata.obj", new Lambertian(Colour(1, 0, 0)));
  s->RotateY(-PI/8 - PI/2);
  // s->Scale(0.4, 0.4, 0.4);
  // s->RotateX(PI);
  s->Translate(3.5, -3, 2.);
  scene->add(s);

  s = new TorusKnotBevel(new Lambertian(Colour(0.55, 0.8, 0.75)), 0.1);
  s->RotateX(PI/2);
  s->Scale(0.8, 0.8, 0.8);
  s->Translate(3, -3, -4);
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
