#include <core/rt.h>

using namespace std;

SCENE(Cornell) {

  Scene *scene = new Scene();
  
  Vec3 e = Vec3(0, 0, -15);
  Vec3 g = -e;
  Vec3 up = Vec3(0, 1, 0);
  scene->cam = Camera(e, g, up, 70, params);

  // params.set<double>("photonRange", .1);
  // params.set<int>("numPhotons", 10000000);
  // params.set<int>("numIterations", 10);

  // scene->integrator = new Path(params);
  // scene->integrator = new DirectLighting(params);
  // scene->integrator = new DebugShader(params);
  // scene->integrator = new PhotonMapping(params);
  // scene->integrator = new PhotonMapping(params);
  scene->integrator = new BaseColour(params);
  
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
  s = new Sphere(new Lambertian(Colour(.75)));
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
  // s = new Sphere(new Lambertian(
  //   new PerlinTexture(Turbulence, 16)
  // ));
  // s->Scale(3.75, 3.75, 3.75);
  // s->Translate(4, -6.25, 6.5);
  // scene->add(s);

  // Middle sphere
  // s = new DisplacedSphere(
  //                         // new PerlinTexture(Layered, 8),
  //                         // new CheckerTexture(),
  //                         new ImageTexture("assets/tex/sand-bump.ppm"),
  //                         new Lambertian(1), 
  //                         .1,   // Scale
  //                         100,  // Resolution
  //                         true  // Interpolate Normals
  //                       );
  // s->addTextureMap(new ImageTexture("assets/tex/sand-bump.ppm"));
  // s->Scale(3.5);
  // s->RotateX(PI/2);
  // s->Translate(-5, -3, 4);
  // scene->add(s);

  // s = new Sphere(new Lambertian(1));
  // s->addTextureMap(new ImageTexture("assets/tex/sand-bump.ppm"));
  // s->addNormalMap(new ImageTexture("assets/tex/sand-norm.ppm"));
  // s->Scale(-4,4,4);
  // s->RotateY(PI);
  // s->Translate(5, -3, 4);
  // scene->add(s);

  // s = new TriangleMesh("assets/obj/water.obj", new Transmissive(1.47, Colour(1)));
  // s->Scale(10, 10, 10);
  // s->Translate(0, -5, 6);
  // scene->add(s);

  double frame = 0;
  double speed = 0.5;
  if (params.exists("frame")) frame = params.get<int>("frame");
  double R = inverseLerp(frame, 0, 60);


  s = new DisplacedPlane(
                          new Perlin4DTexture(Layered, 4, 4, 0.5, R),
                          new Transmissive(1.2, Colour(0.5, 0.7, 1))
                          );
  s->Scale(12, 12, 12);
  s->RotateX(PI/2);
  s->Translate(0, -2, 8);
  scene->add(s);

  // // s = new TriangleMesh("assets/obj/dragon.obj", new Transmissive(1.47, Colour(1)));
  // s = new TriangleMesh("assets/obj/dragon.obj", new Mirror(Colour(1)));
  // // s = new TriangleMesh("assets/obj/dragon.obj", new Lambertian(Colour(0.9)));
  // // s = new TriangleMesh("assets/obj/dragon.obj", new OrenNayar(R, Colour(0.9)));
  // s->Scale(2.5, 2.5, 2.5);
  // s->Translate(0, -10, 6);
  // scene->add(s);

  // s = new TriangleMesh("assets/obj/teapot_lo.obj", 
  //       new Lambertian( 
  //         new ImageTexture("assets/tex/bouncy.ppm")
  //       )
  //     );
  // s->Scale(-10,10,10);
  // s->RotateX(-PI/2);
  // s->Translate(0, 0, 6);
  // scene->add(s);


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


  // s = new TriangleMesh("assets/obj/klein.obj", 
  //                      new Transmissive(1.47, Colour(1)), 
  //                      true);
  // s->Scale(3);
  // // s->RotateX(PI/4);
  // // s->RotateY(PI/4);
  // s->Translate(0, 0, 6);
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

  s = new Disc(new Emitter(Colour(2, 2, 2)));
  s->Scale(2.5);
  s->RotateX(PI/2);
  s->Translate(0,9.9995,5);
  scene->add(s);

  // scene->world = new AGGREGATE(scene->obj_list);
  scene->world = new PrimitiveList(scene->obj_list);
  
  return scene;
}
