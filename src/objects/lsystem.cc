#include "objects/lsystem.h"
#include "core/rt.h"
#include <unordered_map>


void LSystem::createLSystem(char type, int depth, Matrix pTransform,
                            std::vector<Primitive *>& obj_list, RNG& rng) {
  
  if (depth > maxDepth) type = 'd';

  Matrix curTransform;
  Object *curNode;

  if (type == 'a') {
    curNode = new Cylinder(new Transmissive(1.47, Colour(1,.5,.5)));
    curNode->Scale(0.3, 0.3, 1);
  } else if (type == 'b') {
    curNode = new Cylinder(new Transmissive(1.47, Colour(1,.5,.5)));
    curNode->Scale(0.3, 0.3, 1);
  } else if (type == 'c') {
    curNode = new Sphere(new Transmissive(1.47, Colour(.5,1,.5)));
    curNode->Scale(0.1, 0.7, 1);
  } else if (type == 'd') {
    curNode = new Sphere(new Transmissive(1.47, Colour(.5,1,1)));
  }
  curNode->Translate(0, 0, 1);

  double frac = (type == 'a') ? PI/9 : PI/6;
  curTransform = ScaleMatrix(0.8, 0.8, 0.8) * curTransform;
  curTransform = RotateXMatrix(lerp(rng.rand01(), -frac, frac)) * curTransform;
  curTransform = RotateYMatrix(lerp(rng.rand01(), -frac, frac)) * curTransform;
  curTransform = RotateZMatrix(lerp(rng.rand01(), -frac, frac)) * curTransform;
  curTransform = TranslateMatrix(0, 0, 2) * curTransform;
  curTransform = pTransform * curTransform;

  curNode->T = curTransform * curNode->T;
  curNode->finalize();

  obj_list.push_back(curNode);


  char next[25] = "";
  if (type == 'a') strcpy(next, (rng.rand01() < p0) ? "ab" : "bb");
  if (type == 'b') strcpy(next, (rng.rand01() < p1) ? "a" : "c");
  int len = strlen(next);
  for (int i = 0; i < len; i++)
    createLSystem(next[i], depth+1, curTransform, obj_list, rng);
  return;
}

void LSystem::finalize() {
  std::vector<Primitive *> obj_list;
  RNG rng = RNG(seed);
  createLSystem('a', 0, Matrix(), obj_list, rng);
  objs = new BVH(obj_list);
  bounds = objs->bounds;
  Object::finalize();
}

bool LSystem::hit(Ray& r, HitRec &rec) {
  Ray transformed = rayTransform(r);
  if (!objs) {
    printf("Error: `objs` not set for LSystem.\n");
    exit(1);
  }
  if (!objs->hit(transformed, rec)) return false;

  rec.p = r.at(rec.t);
  rec.n = normalTransform(rec.n);
  r.tMax = min(r.tMax, rec.t);

  return true;
}