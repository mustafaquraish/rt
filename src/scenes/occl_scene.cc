#include "core/rt.h"

Scene *OcclusionScene(int sx, int sy) {

  Scene *scene = new Scene();
  scene->sx = sx;
  scene->sy = sy;
  scene->integrator = new AmbientOcclusion();
  
  Vec e = Vec(0, 0, -15);
  Vec g = -e;
  Vec up = Vec(0, 1, 0);
  scene->cam = Camera(e, g, up, -4, -2, 2, 4, sx, sy);

  Object *s;

  s = new TriangleMesh("assets/obj/ev3.obj", new Material(Colour(1, 0, 0)));
  // s->RotateY(PI);
  s->Scale(0.4, 0.4, 0.4);
  s->RotateX(PI);
  s->Translate(-1, -3.5, 3.);
  scene->add(s);

  s = new TriangleMesh("assets/obj/pinata.obj", new Material(Colour(1, 0, 0)));
  s->RotateY(-PI/8 - PI/2);
  // s->Scale(0.4, 0.4, 0.4);
  // s->RotateX(PI);
  s->Translate(3.5, -3, 2.);
  scene->add(s);

  s = new TorusKnotBevel(new Material(Colour(0.55, 0.8, 0.75)), 0.1);
  s->RotateX(PI/2);
  s->Scale(0.8, 0.8, 0.8);
  s->Translate(3, -3, -4);
  scene->add(s);  

  s = new Plane(new Material(Colour(0.55, 0.8, 0.75)));
  s->Scale(20, 20, 20);
  s->RotateZ(PI / 4);
  s->RotateX(PI / 2);
  s->Translate(0, -4, 5);
  scene->add(s);

  scene->world = new AGGREGATE(scene->obj_list);
  
  return scene;
}
