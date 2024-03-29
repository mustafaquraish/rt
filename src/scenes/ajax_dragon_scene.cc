#include <core/rt.h>

SCENE(AjaxDragon) {

  Scene *scene = new Scene();
  
  Vec3 e = Vec3(0, 0, -15);
  Vec3 g = -e;
  Vec3 up = Vec3(0, 1, 0);
  scene->cam = Camera(e, g, up, 55, params);

  // scene->renderer = new DebugShader(params);
  scene->renderer = new AmbientOcclusion(params);

  Object *s;

  s = new TriangleMesh(Simple, "assets/obj/ajax.obj", new Lambertian(Colour(1, 0, 0)));
  s->RotateY(-PI/4 - PI/2);
  s->Scale(0.25, 0.25, 0.25);
  s->RotateY(-PI/8);
  s->Translate(-6, -4, -7);
  scene->add(s);

  s = new TriangleMesh(Simple, "assets/obj/dragon.obj", new Lambertian(Colour(1, 0, 0)));
  s->RotateY(-PI/4);
  s->Scale(2, 2, 2);
  // s->RotateY(-PI/8);
  s->Translate(5, -4, 0);
  scene->add(s);

  s = new Plane(new Lambertian(Colour(0.55, 0.8, 0.75)));
  s->Scale(20, 20, 20);
  s->RotateZ(PI / 4);
  s->RotateX(PI / 2);
  s->Translate(0, -4, 5);
  scene->add(s);

  scene->world = new BVH(scene->obj_list);
  
  return scene;
}
