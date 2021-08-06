#include <core/rt.h>

SCENE(Lsystems) {

  Scene *scene = new Scene();
  
  Vec3 e = Vec3(0, 0, -15);
  Vec3 g = -e;
  Vec3 up = Vec3(0, 1, 0);
  scene->cam = Camera(e, g, up, 53, params);

  scene->integrator = new DebugShader(params);

  Object *s;
  
  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 10; j++) {
      double ioff = lerp(i/10.0, -10., 10.0);
      double joff = lerp(j/10.0, 0., 20.0);
      s = new LSystem(rand() % 4 + 8);
      s->RotateX(-PI/2);
      s->Scale(1, 1.3, 1);
      s->Translate(ioff, -10, joff);
      scene->add(s);
    }
  }
  
  s = new Plane(new Lambertian(Colour(0.55, 0.8, 0.75)));
  s->Scale(20, 20, 20);
  s->RotateX(PI / 2);
  s->Translate(0, -9, 5);
  scene->add(s);

  scene->world = new AGGREGATE(scene->obj_list);
  
  return scene;
}
