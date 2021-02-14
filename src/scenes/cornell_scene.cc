#include "core/rt.h"
#include "materials/emitter.h"
#include "materials/mirror.h"

using namespace std;

SCENE(Cornell) {

  Scene *scene = new Scene();
  
  Vec e = Vec(0, 0, -15);
  Vec g = -e;
  Vec up = Vec(0, 1, 0);
  scene->cam = Camera(e, g, up, -3, params);

  scene->integrator = new Path(params);
  // scene->integrator = new DirectLighting(params);
  // scene->integrator = new DebugShader(params, DEBUG_COLOUR);
  
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
  s = new Sphere(new Lambertian(new CheckerTexture(0.001)));
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

  // // left sphere
  // s = new Sphere(new Transmissive(1.47, Colour(1, 1, 1)));
  // s->Scale(3.75, 3.75, 3.75);
  // s->Translate(-5, -6.25, 4.5);
  // scene->add(s);

  // // right sphere
  // s = new Sphere(new Mirror(Colour(.99, .99, .99)));
  // s->Scale(3.75, 3.75, 3.75);
  // s->Translate(4, -6.25, 6.5);
  // scene->add(s);

  // s = new TriangleMesh("assets/obj/water.obj", new Transmissive(1.47, Colour(1)));
  // s->Scale(10, 10, 10);
  // s->Translate(0, -5, 6);
  // scene->add(s);

  double frame = 0;
  if (params.exists("frame")) frame = params.getInt("frame");
  double R = map(frame, 0, 60, 0, PI);

  // // s = new TriangleMesh("assets/obj/dragon.obj", new Transmissive(1.47, Colour(1)));
  // s = new TriangleMesh("assets/obj/dragon.obj", new Mirror(Colour(1)));
  // // s = new TriangleMesh("assets/obj/dragon.obj", new Lambertian(Colour(0.9)));
  // // s = new TriangleMesh("assets/obj/dragon.obj", new OrenNayar(R, Colour(0.9)));
  // s->Scale(2.5, 2.5, 2.5);
  // s->Translate(0, -10, 6);
  // scene->add(s);

  s = new TriangleMesh("assets/obj/teapot_lo.obj", 
        new Lambertian( 
          new ImageTexture("assets/tex/bouncy.ppm")
        )
      );
  s->Scale(-10,10,10);
  s->RotateX(-PI/2);
  s->Translate(0, 0, 6);
  scene->add(s);


  // Texture *myim = new CheckerTexture();
  // // Texture *myim = new ImageTexture("cornell_is_es.ppm");
  // s = new Sphere(new Transmissive(1.47, Colour(1)));
  // // s = new Sphere(new Lambertian(myim));
  // // s = new Plane(new Lambertian(Colour(0.5)));
  // // s = new TriangleMesh("assets/obj/dragon.obj", new OrenNayar(R, Colour(0.9)));
  // s->Scale(4,4,4);
  // s->RotateX(-PI/4);
  // s->Translate(0, 0, 6);
  // scene->add(s);


  // s = new TriangleMesh("assets/obj/dragon.obj", new Mirror(Colour(1)));
  // s->Scale(1, 1, 1);
  // s->Translate(-4, 0, 6);
  // scene->add(s);

  // s = new TriangleMesh("assets/obj/dragon.obj", new Transmissive(1.47, Colour(1)));
  // s->Scale(.4, .4, .4);
  // s->Translate(-5.5, 4, 6);
  // scene->add(s);

  // s = new LSystem(12);
  // s->Scale(2.5, 2.5, 2.5);
  // s->RotateX(-PI/2);
  // s->RotateY(PI/2);
  // s->Translate(-3, -15, 6);
  // scene->add(s);

  s = new Disc(new Emitter(Colour(12, 12, 12)));
  s->Scale(2.5,2.5,2);
  s->RotateX(PI/2);
  s->Translate(0,9.9995,5);
  scene->add(s);

  // scene->world = new AGGREGATE(scene->obj_list);
  scene->world = new PrimitiveList(scene->obj_list);
  
  return scene;
}
