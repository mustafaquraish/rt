#include "core/rt.h"

using namespace std;

SCENE(Perlin) {

  Scene *scene = new Scene();
  
  Vec e = Vec(0, 0, -15);
  Vec g = -e;
  Vec up = Vec(0, 1, 0);
  scene->cam = Camera(e, g, up, -3, params);

  // scene->integrator = new Path(params);
  scene->integrator = new DebugShader(params);
  // scene->integrator = new DirectLighting(params);
  
  Object *s;

  // Right
  s = new Sphere(new Lambertian(Colour(.25, .25, .75)));
  s->Scale(500, 500, 500);
  s->RotateY(PI / 2);
  s->Translate(510, 0, 5);
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

  s = new TriangleMesh("assets/obj/water.obj", new Transmissive(1.47, Colour(1)));
  s->Scale(10, 20, 10);
  s->Translate(0, -10, 6);
  scene->add(s);

  //  s = new SaddleParametric(new Lambertian(1));
   s = new PerlinTerrain(new Lambertian(1), 100);
  // s = new TorusKnotBevel(new Lambertian(1));
  s->Scale(10, 10, 4);
  s->RotateX(PI/2);
  s->Translate(0,0,5);  
  scene->add(s); 


  s = new Plane(new Emitter(Colour(12, 12, 12)));
  s->Scale(.5,2.5,10);
  s->RotateX(PI/2);
  s->Translate(0,9.9995,5);
  scene->add(s);

  // scene->world = new AGGREGATE(scene->obj_list);
  scene->world = new PrimitiveList(scene->obj_list);
  
  return scene;
}