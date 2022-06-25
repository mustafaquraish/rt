#include <core/rt.h>

using namespace std;

SCENE(A4) {

  Scene *scene = new Scene();

  Vec3 e = Vec3(7, 7, 7);
  Vec3 g = -e;
  Vec3 up = Vec3(0, 1, 0);
  scene->cam = Camera(e, g, up, 30, params);

//   scene->renderer = new Path(params);
  scene->renderer = new DebugShader(params);
  // scene->renderer = new DirectLighting(params);
  // scene->renderer = new BaseColour(params);

  Object *s;

  // Right
  s = new TriangleMesh(MeshType::Simple, "assets/obj/corpse.obj", new Lambertian(1));
  scene->add(s);

//   scene->world = new BVH(scene->obj_list);
    scene->world = new PrimitiveList(scene->obj_list);

  return scene;
}
