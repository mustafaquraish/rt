#include <core/rt.h>

SCENE(Implicit) {

  Scene *scene = new Scene();
  
  Vec3 e = Vec3(0, 0, -15);
  Vec3 g = -e;
  Vec3 up = Vec3(0, 1, 0);
  scene->cam = Camera(e, g, up, 53, params);

  scene->renderer = new DebugShader(params);

  Object *s;

  RNG rng(0);
    
  for (int i = 0; i < 13; i++) {
    float offset = (i/12.0);
    s = new Orthocircle(new Lambertian(Colour(1, .25, .25)), offset);
    s->Scale(0.45, 0.45, 0.45);
    s->RotateZ(rng.rand01() * 2 * PI);
    s->RotateX(rng.rand01() * 2 * PI);
    s->Translate(-7 + i * 1.15, 6.5, 1.5);
    scene->add(s);
  }

  for (int i = 0; i < 13; i++) {
    float offset = (float)(i-6) * 0.3 - 5;
    s = new TangleCube(new Lambertian(Colour(1, .25, .25)), offset);
    s->Scale(0.17, 0.17, 0.17);
    s->RotateZ(rng.rand01() * 2 * PI);
    s->RotateX(rng.rand01() * 2 * PI);
    s->Translate(-7 + i * 1.15, 5, 1.5);
    scene->add(s);
  }

  for (int i = 0; i < 13; i++) {
    float offset = (float)(i-6) * -1;
    s = new Tetrahedral(new Lambertian(Colour(1, .25, .25)), offset);
    s->Scale(0.17, 0.17, 0.17);
    s->RotateZ(rng.rand01() * 2 * PI);
    s->RotateX(rng.rand01() * 2 * PI);
    s->Translate(-7 + i * 1.15, 3.5, 1.5);
    scene->add(s);
  }

  for (int i = 0; i < 13; i++) {
    float offset = (float)(i-6) * -0.01;
    s = new Chubbs(new Lambertian(Colour(1, .25, .25)), offset);
    s->Scale(0.4, 0.4, 0.4);
    s->RotateZ(rng.rand01() * 2 * PI);
    s->RotateX(rng.rand01() * 2 * PI);
    s->Translate(-7 + i * 1.15, 2, 1.5);
    scene->add(s);
  }

  for (int i = 0; i < 13; i++) {
    float offset = (float)(i-6) * -0.01;
    s = new Sweet(new Lambertian(Colour(1, .25, .25)), offset);
    s->Scale(0.4, 0.4, 0.4);
    s->RotateZ(rng.rand01() * 2 * PI);
    s->RotateX(rng.rand01() * 2 * PI);
    s->Translate(-7 + i * 1.15, .5, 1.5);
    scene->add(s);
  }

  for (int i = 0; i < 13; i++) {
    float offset = (float)(i-6) * -0.1;
    s = new Blobs(new Lambertian(Colour(1, .25, .25)), offset);
    s->Scale(0.3, 0.3, 0.3);
    s->RotateZ(rng.rand01() * 2 * PI);
    s->RotateX(rng.rand01() * 2 * PI);
    s->Translate(-7 + i * 1.15, -1, 1.5);
    scene->add(s);
  }

  for (int i = 0; i < 13; i++) {
    float offset = (float)(i-6) * -0.1 + -0.5;
    s = new Bifolia(new Lambertian(Colour(1, .25, .25)), offset);
    s->Scale(0.4, 0.4, 0.4);
    s->RotateZ(rng.rand01() * 2 * PI);
    s->RotateX(rng.rand01() * 2 * PI);
    s->Translate(-7 + i * 1.15, -2.5, 1.5);
    scene->add(s);
  }

  for (int i = 0; i < 13; i++) {
    float offset = (float)(i-6) * -0.03 - 0.1;
    s = new BohemianDome(new Lambertian(Colour(1, .25, .25)), offset);
    s->Scale(0.275, 0.275, 0.275);
    s->RotateZ(rng.rand01() * 2 * PI);
    s->RotateX(rng.rand01() * 2 * PI);
    s->Translate(-7 + i * 1.15, -4, 1.5);
    scene->add(s);
  }

  for (int i = 0; i < 13; i++) {
    float offset = (float)(i-6) * -1;
    s = new Gumdrop(new Lambertian(Colour(1, .25, .25)), offset);
    s->Scale(0.275, 0.275, 0.275);
    s->RotateZ(rng.rand01() * 2 * PI);
    s->RotateX(rng.rand01() * 2 * PI);
    s->Translate(-7 + i * 1.15, -5.5, 1.5);
    scene->add(s);
  }

  for (int i = 0; i < 13; i++) {
    float offset = (i/12.0);
    s = new WiffleCube(new Lambertian(Colour(1, .25, .25)), offset);
    s->Scale(0.17, 0.17, 0.17);
    s->RotateZ(rng.rand01() * 2 * PI);
    s->RotateX(rng.rand01() * 2 * PI);
    s->Translate(-7 + i * 1.15, -7, 1.5);
    scene->add(s);
  }

  scene->world = new AGGREGATE(scene->obj_list);
  
  return scene;
}
