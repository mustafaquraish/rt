#include "core/rt.h"

using namespace std;

SCENE(Orbs) {

  Scene *scene = new Scene();
  
  Vec e = Vec(0, -4, -20);
  Vec g = Vec(0, -2, 0) - e;
  Vec up = Vec(0, 1, 0);
  scene->cam = Camera(e, g, up, -3, params);

  scene->integrator = new Path(params);
  // scene->integrator = new DebugShader(params);
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
  s = new Sphere(new Transmissive(2.2, Colour(.95)));
  s->addNormalMap(new ImageTexture("assets/tex/water-norm.ppm"));
  s->Scale(2);
  s->Translate(-6.5, -8, -5);
  scene->add(s);

  // Center
  // s = new Sphere(new Transmissive(2.2, Colour(.95)));
  s = new TriangleMesh("assets/obj/dragon.obj", new Transmissive(1.47, Colour(.95)));
  // s = new TorusKnotBevel(new Transmissive(2.2, Colour(.95)), 0.15);
  // s->RotateX(PI/2);
  s->Scale(1.5);
  s->Translate(0, -10, -5);
  scene->add(s);

  // Right
  s = new Sphere(new Transmissive(2.2, Colour(.95)));
  s->addNormalMap(new ImageTexture("assets/tex/water-norm.ppm"));
  s->Scale(2);
  s->Translate(6.5, -8, -5);
  scene->add(s);

/******************* Candles *************************************************/

  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {

      if (i == 1 && j == 0) continue;

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

  s = new LSystem(0);
  s->Scale(2.5, 2.5, 2.5);
  s->RotateX(-PI/2);
  s->RotateY(PI/2);
  s->Translate(-8, -15, 1);
  scene->add(s);

  s = new LSystem(0);
  s->Scale(2.5, 2.5, 2.5);
  s->RotateX(-PI/2);
  s->RotateY(-PI/2);
  s->Translate(8, -15, 1);
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
  s->Translate(0,10,5);
  scene->add(s);

  scene->world = new AGGREGATE(scene->obj_list);
  // scene->world = new PrimitiveList(scene->obj_list);
  
  return scene;
}
