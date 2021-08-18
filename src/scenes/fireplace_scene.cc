#include <core/rt.h>

using namespace std;

SCENE(Fireplace) {

  Scene *scene = new Scene();
  
  Vec3 e = Vec3(100, 20, -40);
  Vec3 g = RotateYMatrix(-PI/2.2) * Vec3(0, 0, 1);
  
  Vec3 up = Vec3(0, 1, 0);

  scene->cam = Camera(e, g, up, 70, params);

  scene->renderer = new Path(params);
  // scene->renderer = new DebugShader(params);
  // scene->renderer = new BaseColour(params);
  // scene->renderer = new DirectLighting(params); 
  // scene->renderer = new AmbientOcclusion(params);
  
  Object *s;

  WavefrontOBJ::set_use_mesh_lights(false);
  // WavefrontOBJ::set_mesh_lights_scale(10);

  s = new TriangleMesh(Full, "assets/obj/fireplace_room/fireplace_room.obj");
  s->Scale(20);
  scene->add(s);

  s = new Disc(new Emitter(Colour(.99, .7, .5) * 5));
  s->RotateX(PI/2);
  s->Scale(8, 12, 8);
  s->Translate(30, 56, -35);
  scene->add(s);

  // scene->world = new BVH(scene->obj_list);
  scene->world = new PrimitiveList(scene->obj_list);
  
  return scene;
}
