#include "core/rt.h"

Scene *ImplicitScene(int sx, int sy) {

  Scene *scene = new Scene();
  scene->sx = sx;
  scene->sy = sy;
  scene->integrator = new DebugShader();
  
  Vec e = Vec(0, 0, -15);
  Vec g = -e;
  Vec up = Vec(0, 1, 0);
  scene->cam = Camera(e, g, up, -4, -2, 2, 4, sx, sy);

  Object *s;
    
  for (int i = 0; i < 13; i++) {
    double offset = (i/12.0);
    s = new Orthocircle(new Material(Colour(1, .25, .25)), offset);
    s->Scale(0.45, 0.45, 0.45);
    s->RotateZ(rand01() * 2 * PI);
    s->RotateX(rand01() * 2 * PI);
    s->Translate(-7 + i * 1.15, 6.5, 1.5);
    scene->add(s);
  }

  for (int i = 0; i < 13; i++) {
    double offset = (double)(i-6) * 0.3 - 5;
    s = new TangleCube(new Material(Colour(1, .25, .25)), offset);
    s->Scale(0.17, 0.17, 0.17);
    s->RotateZ(rand01() * 2 * PI);
    s->RotateX(rand01() * 2 * PI);
    s->Translate(-7 + i * 1.15, 5, 1.5);
    scene->add(s);
  }

  for (int i = 0; i < 13; i++) {
    double offset = (double)(i-6) * -1;
    s = new Tetrahedral(new Material(Colour(1, .25, .25)), offset);
    s->Scale(0.17, 0.17, 0.17);
    s->RotateZ(rand01() * 2 * PI);
    s->RotateX(rand01() * 2 * PI);
    s->Translate(-7 + i * 1.15, 3.5, 1.5);
    scene->add(s);
  }

  for (int i = 0; i < 13; i++) {
    double offset = (double)(i-6) * -0.01;
    s = new Chubbs(new Material(Colour(1, .25, .25)), offset);
    s->Scale(0.4, 0.4, 0.4);
    s->RotateZ(rand01() * 2 * PI);
    s->RotateX(rand01() * 2 * PI);
    s->Translate(-7 + i * 1.15, 2, 1.5);
    scene->add(s);
  }

  for (int i = 0; i < 13; i++) {
    double offset = (double)(i-6) * -0.01;
    s = new Sweet(new Material(Colour(1, .25, .25)), offset);
    s->Scale(0.4, 0.4, 0.4);
    s->RotateZ(rand01() * 2 * PI);
    s->RotateX(rand01() * 2 * PI);
    s->Translate(-7 + i * 1.15, .5, 1.5);
    scene->add(s);
  }

  for (int i = 0; i < 13; i++) {
    double offset = (double)(i-6) * -0.1;
    s = new Blobs(new Material(Colour(1, .25, .25)), offset);
    s->Scale(0.3, 0.3, 0.3);
    s->RotateZ(rand01() * 2 * PI);
    s->RotateX(rand01() * 2 * PI);
    s->Translate(-7 + i * 1.15, -1, 1.5);
    scene->add(s);
  }

  for (int i = 0; i < 13; i++) {
    double offset = (double)(i-6) * -0.1 + -0.5;
    s = new Bifolia(new Material(Colour(1, .25, .25)), offset);
    s->Scale(0.4, 0.4, 0.4);
    s->RotateZ(rand01() * 2 * PI);
    s->RotateX(rand01() * 2 * PI);
    s->Translate(-7 + i * 1.15, -2.5, 1.5);
    scene->add(s);
  }

  for (int i = 0; i < 13; i++) {
    double offset = (double)(i-6) * -0.03 - 0.1;
    s = new BohemianDome(new Material(Colour(1, .25, .25)), offset);
    s->Scale(0.275, 0.275, 0.275);
    s->RotateZ(rand01() * 2 * PI);
    s->RotateX(rand01() * 2 * PI);
    s->Translate(-7 + i * 1.15, -4, 1.5);
    scene->add(s);
  }

  for (int i = 0; i < 13; i++) {
    double offset = (double)(i-6) * -1;
    s = new Gumdrop(new Material(Colour(1, .25, .25)), offset);
    s->Scale(0.275, 0.275, 0.275);
    s->RotateZ(rand01() * 2 * PI);
    s->RotateX(rand01() * 2 * PI);
    s->Translate(-7 + i * 1.15, -5.5, 1.5);
    scene->add(s);
  }

  for (int i = 0; i < 13; i++) {
    double offset = (i/12.0);
    s = new WiffleCube(new Material(Colour(1, .25, .25)), offset);
    s->Scale(0.17, 0.17, 0.17);
    s->RotateZ(rand01() * 2 * PI);
    s->RotateX(rand01() * 2 * PI);
    s->Translate(-7 + i * 1.15, -7, 1.5);
    scene->add(s);
  }

  scene->world = new AGGREGATE(scene->obj_list);
  
  return scene;
}