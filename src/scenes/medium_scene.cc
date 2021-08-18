#include <core/rt.h>

using namespace std;

struct MyPerlin : Texture {
  MyPerlin() {}
  virtual Colour get(const HitRec &rec) override {
    Vec3 pt = rec.p / 5;
    float perlin = Simplex::layered(8, 0.5, pt.x, pt.y, pt.z);
    return Simplex::transform(perlin, Layered);
  }
};

struct MyDensity : DensityTexture {
  MyDensity(float intensity, float scale=2) 
    : m_intensity(intensity), m_scale(scale) {}
  virtual float get(const Vec3& p) { 
    Vec3 pt = p * m_scale;
    float perlin = Simplex::layered(8, 0.5, pt.x, pt.y, pt.z);
    return Simplex::transform(perlin, Turbulence) * m_intensity;
  };
  
  float m_intensity;
  float m_scale;
};

struct MyWorleyDensityTexture : DensityTexture {
  MyWorleyDensityTexture(float intensity, float scale=2, float offset=0) 
    : m_intensity(intensity), m_scale(scale) {}
  virtual float get(const Vec3& p) { 
    const float thres = 0.2;
    const Vec3 offvec = {2, 1, 1};
    float n = Worley::layered(p * m_scale + offvec, 1);
    return (max(0, n-thres)) * m_intensity;
  };
  
  float m_intensity;
  float m_scale;
};

SCENE(medium) {

  Scene *scene = new Scene();
  
  Vec3 e = Vec3(0, -5, -15);
  Vec3 g = -e;
  Vec3 up = Vec3(0, 1, 0);
  scene->cam = Camera(e, g, up, 70, params);

  scene->renderer = new Path(params);
  // scene->renderer = new DebugShader(params);
  // scene->renderer = new DirectLighting(params);
  // scene->renderer = new BaseColour(params);
  
  Object *s;

  // Right
  s = new Sphere(new Lambertian(Colour(.25, .25, .75)));
  s->Scale(500, 500, 500);
  s->RotateY(PI / 2);
  s->Translate(510, 0, 5);
  scene->add(s);

  // Left
  s = new Sphere(new Lambertian(Colour(.75, .25, .25)));
  s->Scale(500, 500, 500);
  s->RotateY(-PI / 2);
  s->Translate(-510, 0, 5);
  scene->add(s);

  // Back
  s = new Sphere(new Lambertian(Colour(.95, .95, .95)));
  s->Scale(500, 500, 500);
  s->Translate(0, 0, 515);
  scene->add(s);

  // Bottom
  s = new Sphere(new Lambertian(Colour(.75, .75, .75)));
  s->Scale(500, 500, 500);
  s->RotateX(-PI / 2);
  s->Translate(0, -510, 5);
  scene->add(s);

  // Top
  s = new Sphere(new Lambertian(Colour(.75, .75, .75)));
  s->Scale(500, 500, 500);
  s->RotateX(PI / 2);
  s->Translate(0, 510, 5);
  scene->add(s);


  Object *a, *b;

  double f = params.get<float>("frameRatio");
  
  // a = new Sphere(new Lambertian(Colour(0,1,0)));
  // a->Scale(5, 3.75, 5);
  // a->Translate(0, 2.5, 4.5);  
  // scene->add(a);

  // b = new Cube(new Lambertian(Colour(1, 1, 0)));
  // b->Scale(3);
  // b->RotateY(f*TAU);
  // b->RotateX(-PI/8);
  // b->Translate(0, -4, 4.5);  
  // // scene->add(b);

  // b = new Sphere(new Lambertian(Colour(0,1,0)));
  // b->Scale(5, 3.75, 5);
  // b->Translate(0, -2.5, 4.5);  
  // // scene->add(a);



  // s = new CSGObject(CSGType::Intersection, a, b);
  // s->bsdf = new Transmissive(1.47, Colour(1));
  // // s->bsdf = new Lambertian(Colour(0.5, 1, 0.5));
  // s->RotateY(f*TAU);
  // scene->add(s);




  // s = new ConstantMedium(3,  
  //   new TriangleMesh(Simple, "assets/obj/altostratus00.obj", 
  //                               new Lambertian(Colour(1))))
  // ;
  // s->Scale(0.1, 0.2, 0.1);
  // // s->Translate(0, 0, -3);
  // scene->add(s);

  // s = new Medium(new ConstantDensityTexture(0.01),
  // s = new Medium(new MyDensity(0.1, 3),
  // s = new ConstantMedium(1.5,  
  s = new Medium(new MyWorleyDensityTexture(0.2, 2.5),
    new Cube(new Lambertian(Colour(1.3)))
  );
  s->Scale(10, 2, 10);
  s->Translate(0, 5, 5);
  scene->add(s);

  // s = new Medium(new PerlinDensityTexture(0.05, 2),
  //   new Cube(new Lambertian(Colour(.9, .8, .7)))
  // );
  // s->Scale(5, 5, 1);
  // s->Translate(0, 0, 5);
  // scene->add(s);

  // s = new Cube(new Lambertian(
  //   new MyPerlin()
  //   ));
  // s->Scale(5, 5, 1);
  // s->Translate(0, 0, 5);
  // scene->add(s);

  s = new Disc(new Emitter(Colour(15, 15, 12)));
  s->Scale(3,2.5,3);
  s->RotateX(PI/2);
  s->Translate(0,9.9995,5);
  scene->add(s);

  s = new Disc(new Emitter(Colour(15, 15, 12)));
  s->Scale(3,2.5,3);
  s->RotateY(-PI/2);
  s->Translate(9.99995 ,0,5);
  // scene->add(s);

  s = new Plane(new Emitter(Colour(3)));
  s->Scale(5,5,3);
  // s->RotateX(PI/2);
  s->Translate(0,0,9);
  // scene->add(s);

  // scene->world = new BVH(scene->obj_list);
  scene->world = new PrimitiveList(scene->obj_list);
  
  return scene;
}
