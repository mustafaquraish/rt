#include "core/rt.h"

Scene *SORScene(int sx, int sy) {

  Scene *scene = new Scene();
  scene->sx = sx;
  scene->sy = sy;
  scene->integrator = new DebugShader();
  
  Vec e = Vec(0, 0, -15);
  Vec g = -e;
  Vec up = Vec(0, 1, 0);
  scene->cam = Camera(e, g, up, -4, -2, 2, 4, sx, sy);

  Object *s;

  s = new SinSOR(new Material(Colour(0.55, 0.8, 0.75)));
  s->Scale(.5, .5, .5);
  s->Translate(-4, -4, -3);
  scene->add(s);   

  s = new TorusBevel(new Material(Colour(0.55, 0.8, 0.75)));
  s->Scale(0.5, 0.5, 0.5);
  s->Translate(5, 5, 2);
  scene->add(s); 

  s = new SpiralBevel(new Material(Colour(0.55, 0.8, 0.75)));
  s->Scale(0.5, .5, .5);
  s->Translate(-10, -4, 10);
  scene->add(s);  

  s = new TorusKnotBevel(new Material(Colour(0.55, 0.8, 0.75)), 6, 11);
  s->RotateX(PI/2);
  s->Translate(-5, 5, 2);
  scene->add(s);  

  s = new ParabolicBowlSOR(new Material(Colour(0.55, 0.8, 0.75)));
  s->Translate(5, -4, 10);
  scene->add(s);  

  s = new ButterflyBevel(new Material(Colour(0.55, 0.8, 0.75)));
  s->Translate(0, 0, 2);
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
