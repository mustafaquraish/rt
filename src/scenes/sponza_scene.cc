#include <core/rt.h>

using namespace std;

SCENE(Sponza) {

  Scene *scene = new Scene();
  
  Vec3 e = Vec3(-2, -3, -0.5);
  Vec3 g = RotateYMatrix(PI/2.5) * Vec3(0, 0.3, 1);

  // Vec3 e = Vec3(0, 1, 2);
  // Vec3 g = Vec3(1, 1, 0) - e;
  
  Vec3 up = Vec3(0, 1, 0);

  scene->cam = Camera(e, g, up, 80, params);

  // params.set<int>("numPhotons", 5000000);
  // params.set<float>("photonRange", 0.1);

  scene->renderer = new Path(params);
  // scene->renderer = new PhotonMapping(params);
  // scene->renderer = new SPPM(params);
  // scene->renderer = new DebugShader(params);
  // scene->renderer = new BaseColour(params);
  // scene->renderer = new DirectLighting(params);
  // scene->renderer = new AmbientOcclusion(params);
  
  Object *s;

  s = new RescaledMesh<Full>("assets/obj/sponza/sponza.obj");
  // s->Translate(0, -2, 0);
  s->Scale(20, 20, 20);
  scene->add(s);

  // s = new TriangleMesh<Simple>("assets/obj/living_room.obj", new Lambertian(Colour(.95)));
  // s->Translate(-10, -1, -5);
  // s->Scale(3);
  // scene->add(s);


  s = new Disc(new Emitter(Colour(1)));
  s->RotateX(PI/2);
  s->Scale(10);
  s->Translate(-3, 15, 0);
  scene->add(s);

  // scene->world = new BVH(scene->obj_list);
  scene->world = new PrimitiveList(scene->obj_list);
  
  return scene;
}
