#include <core/rt.h>

using namespace std;

SCENE(salle) {

  Scene *scene = new Scene();
  
  Vec3 e = Vec3(2, 0, -20);
  Vec3 g = RotateXMatrix(PI/20) * RotateYMatrix(-PI/25) * Vec3(0, 0, 1);
  
  Vec3 up = Vec3(0, 1, 0);

  scene->cam = Camera(e, g, up, 45, params); 
 
  scene->renderer = new Path(params);
  // scene->renderer = new DebugShader(params);
  // scene->renderer = new BaseColour(params);
  // scene->renderer = new DirectLighting(params); 
  // scene->renderer = new AmbientOcclusion(params);
  
  Object *s;

  WavefrontOBJ::set_use_mesh_lights(true);
  WavefrontOBJ::set_mesh_lights_scale(2);
  WavefrontOBJ::set_use_hybrid_material(true);

  s = new RescaledMesh(Full, "assets/obj/salle/salle_de_bain.obj");
  s->Scale(20);
  s->RotateY(PI);
  scene->add(s);
 
  // scene->world = new BVH(scene->obj_list);
  scene->world = new PrimitiveList(scene->obj_list);
  
  return scene;
}
