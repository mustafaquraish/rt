#include <core/rt.h>

using namespace std;

SCENE(Env) {
  float frameRatio = params.get<float>("frameRatio");
  Scene *scene = new Scene();
  
  Vec3 e = Vec3(0, 0, -40);
  Vec3 g = -e;
  Vec3 up = Vec3(0, 1, 0);
  scene->cam = Camera(e, g, up, 35, params);

  // params.set("exposure", 0.8);

  scene->renderer = new Path(params);
  // scene->renderer = new DebugShader(params);
  //  scene->renderer = new DirectLighting(params);
  // scene->renderer = new BaseColour(params);
  
  Object *s;

  float R = lerp(frameRatio, 0.0, TAU);
  s = new EnvironmentMap("assets/hdri/snow.ppm");
  s->RotateY(R);
  scene->addEnvMap(s);

  // scene->addEnvMap("assets/tex/water-norm.ppm");
  // scene->addEnvMap(Colour(0.7, 1, 0.7));
  // scene->addEnvMap("assets/hdri/ss.ppm");
  // scene->addEnvMap("assets/hdri/snow.ppm");

  s = new Sphere(new Transmissive(1.47, Colour(0.7)));
  s = new Sphere(new Mirror(Colour(0.9)));
  s->Scale(4);
  s->Translate(-4, 0, 0);
  // scene->add(s);

  s = new Sphere(new Transmissive(1.47, Colour(0.7)));
  s->Scale(4);
  s->Translate(4, 0, 0);
  // scene->add(s);

  s = new TriangleMesh<Simple>("assets/obj/dragon.obj", new Mirror(0.5));
  s->Scale(2.5);
  s->Translate(0,-4,-5);
  scene->add(s);

  s = new Plane(new Lambertian(Colour(0.5)));
  s->addTextureMap(new CheckerTexture(0.01));
  s->Scale(100);
  s->RotateZ(PI / 4);
  s->RotateX(PI / 2);
  s->Translate(0, -4, 5);
  scene->add(s);


  // scene->world = new BVH(scene->obj_list);
  scene->world = new PrimitiveList(scene->obj_list);
  
  return scene;
}
