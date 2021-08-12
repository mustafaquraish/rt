#include <core/rt.h>

using namespace std;

SCENE(Medium) {

  Scene *scene = new Scene();
  
  Vec3 e = Vec3(0, 0, -15);
  Vec3 g = -e;
  Vec3 up = Vec3(0, 1, 0);
  scene->cam = Camera(e, g, up, 70, params);

  scene->renderer = new Path(params);
  // scene->renderer = new DebugShader(params);
  // scene->renderer = new DirectLighting(params);
  // scene->renderer = new BaseColour(params);
  
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

  // left sphere
  s =  new ConstantVolume(0.2, 
    new Sphere(new Lambertian(Colour(1, 1, 0)))
  );
  s->Scale(3.75, 3.75, 3.75);
  s->Translate(-5, -4, 4.5);
  scene->add(s);

  // right sphere
  s = new ConstantVolume(0.2,
    new Cube(new Lambertian(Colour(0, 1, 0)))
  );
  s->Scale(3.75, 3.75, 3.75);
  s->RotateX(PI/5);
  s->RotateZ(PI/8);
  s->Translate(4, -3.75, 6.5);
  scene->add(s);

  s = new ConstantVolume(0.3,
    new TorusSOR(new Lambertian(Colour(0, 0, 1)))
  );
  s->Scale(1.3);
  s->RotateX(PI/5);
  s->RotateZ(PI/8);
  s->Translate(0, +3.75, 6.5);
  scene->add(s);
  
  s = new Plane(new Emitter(Colour(12, 12, 12)*2));
  s->Scale(.5,2.5,10);
  s->RotateX(PI/2);
  s->Translate(0,9.9995,5);
  scene->add(s);

  // scene->world = new BVH(scene->obj_list);
  scene->world = new PrimitiveList(scene->obj_list);
  
  return scene;
}
