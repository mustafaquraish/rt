#include "core/rt.h"

Scene *AjaxScene(int sx, int sy) {

  Scene *scene = new Scene();
  scene->sx = sx;
  scene->sy = sy;
  scene->integrator = new AmbientOcclusion();
  
  Vec e = Vec(0, 0, -15);
  Vec g = -e;
  Vec up = Vec(0, 1, 0);
  scene->cam = Camera(e, g, up, -4, -2, 2, 4, sx, sy);

  Object *s;

  s = new TriangleMesh("assets/obj/ajax.obj", new Material(Colour(1, 0, 0)));
  s->RotateY(-PI/4 - PI/2);
  s->Scale(0.3, 0.3, 0.3);
  // s->RotateX(PI);
  s->Translate(-2, -4, 1.5);
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
