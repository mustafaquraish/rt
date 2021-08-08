#include <core/rt.h>

using namespace std;

SCENE(DOF) {

  Scene *scene = new Scene();
  
  Vec3 e = Vec3(0, 4, -20);
  Vec3 g = Vec3(0, -2, 0) - e;
  Vec3 up = Vec3(0, 1, 0);

  params.set<float>("aperture", 1);
  params.set<float>("focus_dist", 15);

  scene->cam = Camera(e, g, up, 70, params);

  scene->renderer = new Path(params);
  // scene->renderer = new DebugShader(params);
  // scene->renderer = new DirectLighting(params);
  
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
  s->Translate(-6.5,-8,-4);
  scene->add(s);

  // Center
  s = new Sphere(new Transmissive(2.2, Colour(.95)));
  s->Scale(3.6);
  s->Translate(0,-6.4,-5);
  scene->add(s);

  // Right
  s = new Sphere(new Transmissive(2.2, Colour(.95)));
  s->addNormalMap(new ImageTexture("assets/tex/water-norm.ppm"));
  s->Scale(2);
  s->Translate(6.5,-8,-4);
  scene->add(s);

/******************* Candles *************************************************/

  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {

      if (i == 1 && j == 0) continue;

      float xOff = -6.5 + i * 6.5;
      float zOff =    1 + j * 4;

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
