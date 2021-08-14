#include <core/rt.h>

using namespace std;

SCENE(csg) {

  Scene *scene = new Scene();
  
  Vec3 eye = Vec3(0, 0, -15);
  Vec3 g = -eye;
  Vec3 up = Vec3(0, 1, 0);
  scene->cam = Camera(eye, g, up, 70, params);

  scene->renderer = new Path(params);
  // scene->renderer = new DebugShader(params);
  // scene->renderer = new DirectLighting(params);
  // scene->renderer = new BaseColour(params);
  
  Object *s;

  float frameRatio = params.get<float>("frameRatio");

  // Right
  s = new Sphere(new Lambertian(Colour(.25, .25, .75)));
  s->Scale(500, 500, 500);
  s->RotateY(PI / 2);
  s->Translate(510, 0, 5);
  scene->add(s);

  // Left
  s = new Sphere(new Lambertian(Colour(.75, .25, .25)));
  s->Scale(500, 500, 500);
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
  s->Translate(0, -510, 5);
  scene->add(s);

  // Top
  s = new Sphere(new Lambertian(Colour(.75, .75, .75)));
  s->Scale(500, 500, 500);
  s->Translate(0, 510, 5);
  scene->add(s);


  /**
   * Creating the Diagram from wikipedia page:
   * https://en.wikipedia.org/wiki/Constructive_solid_geometry
   */

  Object *a, *b, *c, *d, *e;

  a = new Cube(new Lambertian(Colour(1, 0.5, 0.5)));
  a->Scale(3);
  
  b = new Sphere(new Lambertian(Colour(0, 0, 1)));
  b->Scale(4);

  c = new Cylinder(new Lambertian(Colour(0, 1, 0)));
  c->Scale(2, 2, 4);
  // scene->add(c);

  d = new Cylinder(new Lambertian(Colour(0, 1, 0)));
  d->Scale(2, 2, 4);
  d->RotateY(PI/2);

  e = new Cylinder(new Lambertian(Colour(0, 1, 0)));
  e->Scale(2, 2, 4);
  e->RotateX(PI/2);

  s = new CSGObject(Difference,
        new CSGObject(Intersection, a, b),
        new CSGObject(Union,
          new CSGObject(Union, c, d),
          e
        )
      );
  s->RotateX(frameRatio*TAU);
  s->RotateZ(frameRatio*PI);
  // s->bsdf = new Transmissive(1.47, Colour(1));
  s->Translate(0, 0, 5);
  scene->add(s);

  s = new Plane(new Emitter(Colour(12, 12, 12)));
  s->Scale(.5,2.5,10);
  s->RotateX(PI/2);
  s->Translate(0,9.9995,5);
  scene->add(s);

  // scene->world = new BVH(scene->obj_list);
  scene->world = new PrimitiveList(scene->obj_list);
  
  return scene;
}
