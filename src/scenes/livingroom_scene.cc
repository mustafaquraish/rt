#include <core/rt.h>

using namespace std;

SCENE(Living) {

  Scene *scene = new Scene();
  

  Vec3 e = Vec3(-55, -15, 140);
  Vec3 g = RotateYMatrix(-PI/6.2) * Vec3(0, -0.06, -1);
  // Vec3 g = Vec3(10,-40,0) - e;

  // Vec3 e = Vec3(0, 1, 2);
  // Vec3 g = Vec3(1, 1, 0) - e;
  

  Vec3 up = Vec3(0, 1, 0);

  scene->cam = Camera(e, g, up, 46, params);

  // params.set_window(0.43, 0.55, 0.04, 0.22);
  scene->renderer = new Path(params);
  // scene->renderer = new DebugShader(params);
  // scene->renderer = new BaseColour(params);
  // scene->renderer = new DirectLighting(params);
  // scene->renderer = new AmbientOcclusion(params);
  
  Object *s;

  // WavefrontOBJ::set_use_mesh_lights(true);
  WavefrontOBJ::set_use_hybrid_material(true);
  // WavefrontOBJ::set_mesh_lights_scale(1);

  s = new TriangleMesh(Full, "assets/obj/living_room/living_room.obj");
  s->Translate(0, -2, 0);
  s->Scale(20, 20, 20);
  scene->add(s);

  // s = new TriangleMesh(Simple, "assets/obj/living_room.obj", new Lambertian(Colour(.95)));
  // s->Translate(-10, -1, -5);
  // s->Scale(3);
  // scene->add(s);


  s = new Plane(new Emitter(Colour(.99) * 20));
  // s->RotateX(PI/2);
  s->Scale(32, 15, 1);
  s->Translate(-1, -4, 9);
  scene->add(s);

  // scene->world = new BVH(scene->obj_list);
  scene->world = new PrimitiveList(scene->obj_list);
  
  return scene;
}
