#include <core/rt.h>

float mysdf1(const Vec3& point) {
  Vec3 p = point;
  if (p.z < 10) p.z = 10;
  p = SDF::repeatX(p, 5);
  p = SDF::repeatY(p, 5);
  p = SDF::repeatZ(p, 5);
  return SDF::tetrahedron(p);
}

SCENE(SDF) {

  Scene *scene = new Scene();
  
  Vec3 e = Vec3(0, 0, -15);
  Vec3 g = -e;
  Vec3 up = Vec3(0, 1, 0);
  scene->cam = Camera(e, g, up, 50, params);

  // params.set_window(0.2, 0.4, 0.4, 0.6);

  // scene->renderer = new DebugShader(params);
  // scene->renderer = new AmbientOcclusion(params);
  scene->renderer = new BaseColour(params);


  float frameRatio = params.get<float>("frameRatio");

  Object *s;

  // s = new SphereSDF(new Lambertian(1));
  // scene->add(s);

  // float R = lerp(frameRatio, 0.0, 4.0);
  // s = new InfiniteSphereSDF(new Lambertian(1));
  // s->addTextureMap(new SDFTexture(Colour(1), 10));
  // s->Translate(R, R, 0);
  // scene->add(s);

  // s = new SerpinksiSDF(new Lambertian(1));
  // s->addTextureMap(new SDFTexture(Colour(1), 10));
  // s->Scale(3.5);
  // s->RotateY(PI/4);
  // s->RotateX(PI + PI/4);
  // s->Translate(0, -1, 0);
  // scene->add(s);

  float R = lerp(frameRatio, 0.0, PI);
  float power = lerp(frameRatio, 5.0, 15.0);
  s = new MandelBulbSDF(new Lambertian(1), power);
  s->addTextureMap(new SDFTexture(Colour(1), 3.5));
  // s->RotateX(PI/2);
  s->RotateX(-PI/2 + R);
  s->RotateZ(R);
  s->Scale(5);
  scene->add(s);

  // s = new CustomSDF(mysdf1, new Lambertian(1));
  // s->addTextureMap(new SDFTexture(Colour(1), 10));
  // scene->add(s);

  scene->world = new PrimitiveList(scene->obj_list);
  
  return scene;
}
