#include "core/rt.h"

Scene *A2Scene(int sx, int sy) {

  Scene *scene = new Scene();
  scene->sx = sx;
  scene->sy = sy;
  scene->integrator = new DebugShader();
  
  Vec e = Vec(0, 0, -1);
  Vec g = -e;
  Vec up = Vec(0, 1, 0);
  scene->cam = Camera(e, g, up, -1, -2, 2, 4, sx, sy);

  // PrimitiveList world = PrimitiveList();
  // std::vector<PointLS> ls = std::vector<PointLS>();
  Object *s;

  s = new TangleCube(new Material(Colour(1, .25, .25)));
  s->Scale(0.2, 0.2, 0.2);
  // s->Scale(1.5, .75, .75);
  s->RotateZ(PI / 4);
  s->Translate(2.0, 2.5, 1.5);
  scene->add(s);
  // world.add(s);

  s = new Sphere(new Material(Colour(0.75, 0.95, 0.55)));
  s->Scale(0.95, 1.65, .65);
  s->RotateZ(-PI / 1.5);
  s->Translate(-2.2, 1.75, 1.35);
  scene->add(s);
  // world.add(s);

  s = new TriangleMesh("assets/obj/pinata.obj", new Material(Colour(1, 0, 0)));
  // s->RotateY(PI);
  // s->RotateX(PI/2);
  s->Translate(0,0,2);
  scene->add(s);

  for (int i = 0; i < 1000000; i++) {
    s = new Sphere(new Material(Colour(0.75, 0.95, 0.55)));
    s->Scale(.05, .05, .05);
    s->RotateZ(-PI / 1.5);
    s->Translate(rand01()*30-15, rand01()*30-15, rand01()+3);
    s->Scale(1, 1, 5);
    s->Translate(0, 0, -10);
    scene->add(s);
  }

  s = new Sphere(new Material(Colour(0.55, 0.55, 0.95)));
  s->both = 1;
  s->Scale(100, 100, 100);
  s->RotateZ(-PI / 1.5);
  s->Translate(0,0, 0);
  // scene->add(s);
  // world.add(s);

  s = new Plane(new Material(Colour(0.55, 0.8, 0.75)));
  s->Scale(11, 11, 11);
  s->RotateZ(PI / 4);
  s->RotateX(PI / 2);
  s->Translate(0, -4, 5);
  scene->add(s);

  scene->world = new AGGREGATE(scene->obj_list);
  
  return scene;
}
