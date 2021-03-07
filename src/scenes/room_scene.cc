#include "core/rt.h"

using namespace std;

SCENE(Room) {

  Scene *scene = new Scene();
  
  Vec e = Vec(0, -4, -40);
  Vec g = Vec(0, -1.2, 0) - e;
  Vec up = Vec(0, 1, 0);

  double dist = length(Vec(-2, -2, -7.5) - e);
  // double dist = length(Vec(-6.5, -9, 1) - e);

  params.set<double>("aperture", .5);
  params.set<double>("focus_dist", 35);


  scene->cam = Camera(e, g, up, 32, params);

  scene->integrator = new Path(params);
  // scene->integrator = new DebugShader(params);
  // scene->integrator = new BaseColour(params);
  // scene->integrator = new DirectLighting(params);
  
  Object *s;

  // Back
  s = new Plane(new Lambertian(Colour(.95, 0, 0)));
  s->addTextureMap(new ImageTexture("assets/tex/brick-texture.ppm"));
  s->addNormalMap(new ImageTexture("assets/tex/brick-norm.ppm"));
  s->Scale(30, 15, 1);
  s->RotateX(PI);
  s->Translate(-15, 0, 20);
  scene->add(s);

  s = new Plane(new Lambertian(Colour(.95, 0, 0)));
  s->addTextureMap(new ImageTexture("assets/tex/brick-texture.ppm"));
  s->addNormalMap(new ImageTexture("assets/tex/brick-norm.ppm"));
  s->Scale(30, 15, 1);
  s->RotateX(PI);
  s->Translate(15, 0, 20);
  scene->add(s);

  // Bottom
  s = new Plane(new Lambertian(Colour(.75, .75, .75)));
  s->addTextureMap(new ImageTexture("assets/tex/wood-texture.ppm"));
  s->addNormalMap(new ImageTexture("assets/tex/wood-norm.ppm"));
  s->Scale(50);
  s->RotateX(-PI / 2);
  s->RotateY(PI / 2);
  s->Translate(0, -10, 5);
  scene->add(s);

  // Top
  s = new Plane(new Lambertian(Colour(.75, .75, .75)));
  s->Scale(50);
  s->RotateX(PI / 2);
  s->Translate(0, 10, 5);
  scene->add(s);

/******************* ORBS ****************************************************/

  // Left 
  s = new DisplacedSphere(
    new ImageTexture("assets/tex/sand-bump.ppm"),
    new Transmissive(2.2, Colour(.95))
  );
  s->Scale(2);
  s->RotateX(PI/2);
  s->RotateY(-PI/2);
  s->Translate(-10, -7.8, -5);
  scene->add(s);

  // Center
  // s = new Sphere(new Transmissive(2.2, Colour(.95)));
  s = new TriangleMesh("assets/obj/dragon.obj", new Transmissive(1.47, Colour(.95)));
  s->Scale(2);
  s->Translate(0, -10, -5);
  scene->add(s);

  // Right
  s = new TangleCube(new Transmissive(2.2, Colour(.95)));
  // s->Scale(2);
  s->Translate(10, -7.7, -5);
  scene->add(s);

/******************* Candles *************************************************/

  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {

      // if (i == 1 && j == 0) continue;

      double xOff = -6.5 + i * 6.5;
      double zOff =    1 + j * 4;

      s = new Cylinder(new Lambertian(1));
      s->Scale(2, 1, 1);
      s->RotateX(PI/2);
      s->Translate(xOff, -10, zOff);
      scene->add(s);

      s = new Sphere(new Emitter( Colour(1, .6, .3)*30 ));
      s->Scale(.2, .5, .2);
      s->Translate(xOff, -9, zOff);
      scene->add(s);
    }
  }

  // srand(time(NULL));
  // int seed = rand();
  // printf("========== Seed is : %d\n", seed);

  int seed = 1217349511;

  s = new LSystem(seed);
  s->Scale(2.5, 2.5, 2.5);
  s->RotateX(-PI/2);
  // s->RotateY(-PI/8);
  s->RotateZ(-PI/15);
  s->Translate(-13, -14, 1);
  scene->add(s);

  s = new LSystem(seed);
  s->Scale(2.5, 2.5, 2.5);
  s->RotateX(-PI/2);
  // s->RotateY(-PI/8);
  s->RotateZ(-PI/15);
  s->Scale(-1, 1, 1);
  s->Translate(13, -14, 1);
  scene->add(s);


   ///////////////////// A1 ///////////////////////
 s = new Plane(new Lambertian(1));
 s->addTextureMap(new ImageTexture("assets/tex/A1.ppm"));
 s->Scale(4,2.5,1);
 s->Translate(-6.666,1.5,19.9);
 scene->add(s);
//  loadTexture( "textures/A1.ppm", 1, &texture_list);

 s = new Plane(new Lambertian(.2));
 s->Scale(4.3,2.8,1);
 s->Translate(-6.666,1.5,19.95);
 scene->add(s);

 ///////////////////// A2 ///////////////////////
 s = new Plane(new Lambertian(1));
 s->addTextureMap(new ImageTexture("assets/tex/A2.ppm"));
 s->Scale(4,4,1);
 s->Translate(-20,0,19.9);
 scene->add(s);
//  loadTexture( "textures/A2.ppm", 1, &texture_list);

 s = new Plane(new Lambertian(.2));
 s->Scale(4.3,4.3,1);
 s->Translate(-20,0,19.95);
 scene->add(s);

 ///////////////////// A3 ///////////////////////
 s = new Plane(new Lambertian(1));
 s->addTextureMap(new ImageTexture("assets/tex/A3.ppm"));
 s->Scale(4,4,1);
 s->Translate(6.666,0,19.9);
//  loadTexture( "textures/A3.ppm", 1, &texture_list);
 scene->add(s);

 s = new Plane(new Lambertian(.2));
 s->Scale(4.3,4.3,1);
 s->Translate(6.666,0,19.95);
 scene->add(s);

 ///////////////////// A4 ///////////////////////
 s = new Plane(new Lambertian(1));
 s->addTextureMap(new ImageTexture("assets/tex/A4.ppm"));
 s->Scale(4,4,1);
 s->Translate(20,0,19.9);
//  loadTexture( "textures/A4.ppm", 1, &texture_list);
 scene->add(s);

 s = new Plane(new Lambertian(.2));
 s->Scale(4.3,4.3,1);
 s->RotateX(PI);
 s->Translate(20,0,19.95);
 scene->add(s);

/******************* Light ****************************************************/

  s = new Disc(new Emitter(Colour(.99, .7, .5)*15));
  s->Scale(3);
  s->RotateX(PI/2);
  s->Translate(0,9.99,5);
  // scene->add(s); 


  // TODO: Figure out where the sphere light breaks everything??

  s = new Sphere(new Emitter(Colour(.99, .7, .5)*15));
  s->Scale(3, 1, 3);
  // s->RotateX(PI/2);
  s->Translate(0,9.5,5);
  scene->add(s);

  scene->world = new AGGREGATE(scene->obj_list);
  // scene->world = new PrimitiveList(scene->obj_list);
  
  return scene;
}
