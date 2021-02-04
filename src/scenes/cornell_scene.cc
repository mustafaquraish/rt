#include "core/rt.h"
#include "materials/emitter.h"
#include "materials/mirror.h"

using namespace std;

Scene *CornellScene(int sx, int sy) {

  Scene *scene = new Scene();
  scene->sx = sx;
  scene->sy = sy;
  scene->integrator = new Path();
  // scene->integrator = new DebugShader();
  
  Vec e = Vec(0, 0, -15);
  Vec g = -e;
  Vec up = Vec(0, 1, 0);
  scene->cam = Camera(e, g, up, -3, -2, 2, 4, sx, sy);

  Object *s;

  // Right
  s = new Sphere(new Lambertian(Colour(.25, .25, .75)));
  s->Scale(500, 500, 500);
  s->RotateY(PI / 2);
  s->Translate(510, 0, 5);
  cout << s->T << endl;
  scene->add(s);

  // Left
  s = new Sphere(new Lambertian(Colour(.75, .25, .25)));
  s->Scale(500, 500, 500);
  s->RotateY(-PI / 2);
  s->Translate(-510, 0, 5);
  scene->add(s);

  // Back
  s = new Sphere(new Lambertian(Colour(.95, .95, .95)));
  s->Scale(500, 500, 500);
  s->Translate(0, 0, 515);
  scene->add(s);

  // Bottom
  s = new Sphere(new Lambertian(Colour(.75, .75, .75)));
  s->Scale(500, 500, 500);
  s->RotateX(-PI / 2);
  s->Translate(0, -510, 5);
  scene->add(s);

  // Top
  s = new Sphere(new Lambertian(Colour(.75, .75, .75)));
  s->Scale(500, 500, 500);
  s->RotateX(PI / 2);
  s->Translate(0, 510, 5);
  scene->add(s);

  // left sphere
  s = new Sphere(new Transmissive(1.47, Colour(1, 1, 1)));
  s->Scale(3.75, 3.75, 3.75);
  s->Translate(-5, -4, 4.5);
  scene->add(s);

  // right sphere
  s = new Sphere(new Mirror(Colour(.99, .99, .99)));
  s->Scale(3.75, 3.75, 3.75);
  s->Translate(4, -3.75, 6.5);
  scene->add(s);

  s = new Disc(new Emitter(Colour(12, 12, 12)));
  s->Scale(2.5,2.5,2);
  s->RotateX(PI/2);
  s->Translate(0,9.995,5);
  scene->add(s);

  // scene->world = new AGGREGATE(scene->obj_list);
  scene->world = new PrimitiveList(scene->obj_list);
  
  return scene;
}
