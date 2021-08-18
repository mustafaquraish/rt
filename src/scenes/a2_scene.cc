#include <core/rt.h>

SCENE(A2) {

  Scene *scene = new Scene();
  
  Vec3 e = Vec3(0, 0, -1);
  Vec3 g = -e;
  Vec3 up = Vec3(0, 1, 0);
  scene->cam = Camera(e, g, up, 125, params);

  scene->renderer = new BaseColour(params);
  
  Object *s;

  // Plane at the bottom
  s = new Plane(
    new Lambertian(Colour(.55,.8,.75))
  );
  s->Scale(11,11,11);
  s->RotateZ(PI/4);
  s->RotateX(PI/2);
  s->Translate(0,-4,5);
  scene->add(s);

  // Teapot...
  s = new TriangleMesh(Simple, "assets/obj/teapot_lo.obj", 
    new Lambertian(new ImageTexture("assets/tex/bouncy.ppm"))
  );
  s->Scale(3);
  s->RotateX(-PI/2);
  s->Translate(0,-2,4);
  scene->add(s);

  /*************************************************************************/

  // left sphere
  s = new Sphere(
    new Lambertian(Colour(1,.25,.25))
  );
  s->Scale(1.5,.75,.75);
  s->RotateZ(PI/4);					
  s->Translate(2.0,2.5,1.5);
  scene->add(s);

  // right sphere
  s = new Sphere(
    new Lambertian(Colour(.75,.95,.55))
  );
  s->Scale(.95,1.65,.65);
  s->RotateZ(-PI/1.5);
  s->Translate(-2.2,1.75,1.35);
  scene->add(s);
  
  /*************************************************************************/

  // scene->add(
  //   PointLight(Vec3(0, 25.5, -3.5), Colour(0.95))
  // );

  scene->world = new PrimitiveList(scene->obj_list);
  
  return scene;
}
