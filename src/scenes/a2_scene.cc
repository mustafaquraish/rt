#include "core/rt.h"

Scene *A2Scene(int sx, int sy) {

  Scene *scene = new Scene();
  scene->sx = sx;
  scene->sy = sy;
  scene->integrator = new DebugShader();
  
  Vec e = Vec(0, 0, -15);
  Vec g = -e;
  Vec up = Vec(0, 1, 0);
  scene->cam = Camera(e, g, up, -4, -2, 2, 4, sx, sy);

  // PrimitiveList world = PrimitiveList();
  // std::vector<PointLS> ls = std::vector<PointLS>();
  Object *s;

   for (int i = -4; i < 5; i++) {
    for (int j = -3; j < 4; j++) {
      s = new TriangleMesh("assets/obj/pinata.obj", new Material(Colour(1, 0, 0)));
      s->Scale(0.2, 0.2, 0.2);
      s->Translate((double)i * 1.5,(double)(j + 4)*.3,3. + (double)j * 1);
      scene->add(s);
    }
  }

  for (int i = -4; i < 5; i++) {
    for (int j = -6; j < 4; j++) {
      s = new TriangleMesh("assets/obj/ev3.obj", new Material(Colour(1, 0, 0)));
      // s->RotateY(PI);
      s->Scale(0.1, 0.1, 0.1);
      s->RotateX(PI);
      s->Translate((double)i * 1.5 + .5,
                   -2 + (double)(j - 4)*0.1, 
                    3. + (double)j * 1);
      scene->add(s);
    }
  }

  for (int i = 0; i < 13; i++) {
    if (i % 5 == 0) { s = new TangleCube(new Material(Colour(1, .25, .25)));}
    if (i % 5 == 1) { s = new Torus(new Material(Colour(1, .25, .25)));}
    if (i % 5 == 2) { s = new Tetrahedral(new Material(Colour(1, .25, .25)));}
    if (i % 5 == 3) { s = new Chubbs(new Material(Colour(1, .25, .25))); s->Scale(2,2,2);}
    if (i % 5 == 4) { s = new Sweet(new Material(Colour(1, .25, .25))); s->Scale(2.5,2.5,2.5);}
    
    s->Scale(0.2, 0.2, 0.2);
    // s->Scale(1.5, .75, .75);
    s->RotateZ(rand01() * 2 * PI);
    s->RotateX(rand01() * 2 * PI);
    s->Translate(-7 + i * 1.15, 5, 1.5);
    scene->add(s);
  }

  for (int i = 0; i < 13; i++) {
    if (i % 4 == 0) s = new Sphere(new Material(Colour(1, .25, .25)));
    if (i % 4 == 1) s = new Cylinder(new Material(Colour(1, .25, .25)));
    if (i % 4 == 2) s = new Plane(new Material(Colour(1, .25, .25)));
    if (i % 4 == 3) s = new Disc(new Material(Colour(1, .25, .25)));

    s->Scale(0.3+rand01()*0.2, 0.3+rand01()*0.2, 0.3+rand01()*0.2);
    // s->Scale(1.5, .75, .75);
    s->RotateZ(rand01() * 2 * PI);
    s->RotateX(rand01() * 2 * PI);
    s->Translate(-7 + i * 1.15, 6.5, 1.5);
    scene->add(s);
  }

  for (int i = 0; i < 13; i++) {
    s = new Cube(new Material(Colour(1, .25, .25)));
    s->Scale(0.4, 0.4, 0.4);
    // s->Scale(1.5, .75, .75);
    s->RotateZ(rand01() * 2 * PI);
    s->RotateX(rand01() * 2 * PI);
    s->Translate(-7 + i * 1.15, 3.5, 1.5);
    scene->add(s);
  }


  for (int i = 0; i < 500000; i++) {
    s = new Sphere(new Material(Colour(0.75, 0.95, 0.55)));
    s->Scale(.05, .05, .05);
    s->RotateZ(-PI / 1.5);
    s->Translate(rand01()*60-30, rand01()*40 - 10, rand01() + 10);
    s->Scale(1, 1, 5);
    s->Translate(0, 0, -10);
    scene->add(s);
  }

  s = new Plane(new Material(Colour(0.55, 0.8, 0.75)));
  s->Scale(20, 20, 20);
  s->RotateZ(PI / 4);
  s->RotateX(PI / 2);
  s->Translate(0, -4, 5);
  scene->add(s);

  scene->world = new AGGREGATE(scene->obj_list);
  
  return scene;
}
