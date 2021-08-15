#include <core/rt.h>

using namespace std;

SCENE(Scratch) {

  Scene *scene = new Scene();
  

  Vec3 e = Vec3(5.03f, 0.91f, -2.20f);
  Vec3 g = Vec3(-0.21f, 1.0f, -0.34f) - e;

  // Vec3 e = Vec3(0, 1, 2);
  // Vec3 g = Vec3(1, 1, 0) - e;
  
  Vec3 up = Vec3(0, 1, 0);

  scene->cam = Camera(e, g, up, 45, params);

  // scene->renderer = new Path(params);
  // scene->renderer = new DebugShader(params);
  // scene->renderer = new BaseColour(params);
  scene->renderer = new DirectLighting(params);
  // scene->renderer = new AmbientOcclusion(params);
  
  Object *s;

  s = new TriangleMesh(Simple, "assets/obj/square.obj");
  scene->add(s);

  // s = new TriangleMesh(Simple, "assets/obj/living_room.obj", new Lambertian(Colour(.95)));
  // s->Translate(-10, -1, -5);
  // s->Scale(3);
  // scene->add(s);


  s = new Sphere(new Emitter(Colour(.99, .7, .5) * 30));
  s->Scale(1, 1, 1);
  // s->RotateX(PI/2);
  s->Translate(5.03f, 2.91f, -2.20f);
  scene->add(s);

  // scene->world = new BVH(scene->obj_list);
  scene->world = new PrimitiveList(scene->obj_list);
  
  return scene;
}
