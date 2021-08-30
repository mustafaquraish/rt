#include <core/rt.h>

using namespace std;

SCENE(bf) {

  Scene *scene = new Scene();
  
  Vec3 e = Vec3(0, 0, -10);
  Vec3 g = Vec3(0, -.2, 1);
  
  Vec3 up = Vec3(0, 1, 0);

  scene->cam = Camera(e, g, up, 40, params); 
 
  scene->renderer = new Path(params);
  // scene->renderer = new DebugShader(params);
  // scene->renderer = new BaseColour(params);
  // scene->renderer = new DirectLighting(params); 
  // scene->renderer = new AmbientOcclusion(params);
   
  Object *s;

  // WavefrontOBJ::set_use_mesh_lights(true);
  // WavefrontOBJ::set_mesh_lights_scale(50);
  WavefrontOBJ::set_use_hybrid_material(true);

  s = new RescaledMesh(Full, "assets/obj/breakfast/breakfast_room.obj", nullptr, true);
  s->Scale(20);
  s->RotateY(PI);
  scene->add(s);

  s = new Plane(new Emitter(Colour(.99) * 10));
  s->Scale(5.1, 3.5, 1);
  s->RotateY(PI/2);
  s->Translate(8, 1.5, 5);
  scene->add(s);
 
  // scene->world = new BVH(scene->obj_list);
  scene->world = new PrimitiveList(scene->obj_list);
  
  return scene;
}
