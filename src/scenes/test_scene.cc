#include <core/rt.h>

using namespace std;

SCENE(Test) {

  Scene *scene = new Scene();
  
  Vec3 e = Vec3(0, 0, -15);
  Vec3 g = -e;
  Vec3 up = Vec3(0, 1, 0);
  scene->cam = Camera(e, g, up, 70, params);

  params.set<float>("photonRange", .2);
  params.set<int>("numPhotons", 10000);
  params.set<int>("numIterations", 10);

  // scene->renderer = new Path(params);
  // scene->renderer = new PhotonMapping(params);
  scene->renderer = new SPPM(params);
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

  // // blocking
  // s = new Plane(new Lambertian(Colour(.75)));
  // s->Scale(5, 5, 5);
  // s->RotateX(-PI / 2);
  // s->Translate(0, 8, 5);
  // scene->add(s);

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

  // s = new TangleCube(new Transmissive(1.47, Colour(1, 1, 1)));
  // s->RotateX(PI / 6);
  // s->RotateY(-PI / 9);
  // s->Scale(2);
  // s->Translate(0, 0, 4.5);
  // scene->add(s);

  // s = new DisplacedPlane(
  //                         new Perlin4DTexture(Layered, 4, 4, 0.5, 0),
  //                         new Transmissive(1.2, Colour(0.5, 0.7, 1))
  //                         );
  // s->Scale(12, 12, 12);
  // s->RotateX(PI/2);
  // s->Translate(0, -2, 8);
  // scene->add(s);
  
  s = new Plane(new Emitter(Colour(1, 1, 1)));
  s->Scale(.5,2.5,10);
  s->RotateX(PI/2);
  s->Translate(0,9.9995,5);
  scene->add(s);

  // s = new Disc(new Emitter(Colour(1, 1, 1)));
  // s->Scale(2.5,2.5,10);
  // s->RotateX(PI/2);
  // s->Translate(0,9.9995,5);
  // scene->add(s);

  // scene->world = new AGGREGATE(scene->obj_list);
  scene->world = new PrimitiveList(scene->obj_list);
  
  return scene;
}
