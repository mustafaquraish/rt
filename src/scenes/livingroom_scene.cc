#include <core/rt.h>

using namespace std;

SCENE(Living) {

  Scene *scene = new Scene();
  

  Vec3 e = Vec3(-30, 1, 120);
  Vec3 g = RotateYMatrix(-PI/10) * Vec3(0, -.4, -1);

  // Vec3 e = Vec3(0, 1, 2);
  // Vec3 g = Vec3(1, 1, 0) - e;
  
  Vec3 up = Vec3(0, 1, 0);

  scene->cam = Camera(e, g, up, 70, params);

  scene->renderer = new Path(params);
  // scene->renderer = new DebugShader(params);
  // scene->renderer = new BaseColour(params);
  // scene->renderer = new DirectLighting(params);
  // scene->renderer = new AmbientOcclusion(params);
  
  Object *s;

  WavefrontOBJ::set_use_mesh_lights(false);

  s = new TriangleMesh(Full, "assets/obj/living_room/living_room.obj");
  s->Translate(0, -2, 0);
  s->Scale(20, 20, 20);
  scene->add(s);

  // s = new TriangleMesh(Simple, "assets/obj/living_room.obj", new Lambertian(Colour(.95)));
  // s->Translate(-10, -1, -5);
  // s->Scale(3);
  // scene->add(s);


  s = new Disc(new Emitter(Colour(.99, .7, .5) * 10));
  s->RotateX(PI/2);
  s->Scale(10, 10, 10);
  s->Translate(35, 22, 70);
  scene->add(s);

  // scene->world = new BVH(scene->obj_list);
  scene->world = new PrimitiveList(scene->obj_list);
  
  return scene;
}
