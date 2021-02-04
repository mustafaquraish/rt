#include "objects/lsystem.h"

void LSystem::createLSystem(char type, int depth, Matrix pTransform,
                            std::vector<Primitive *>& obj_list) {
  
  if (depth > maxDepth) type = 'd';

  Matrix curTransform;
  Object *curNode;

  if (type == 'a') {
    curNode = new Cylinder(new Lambertian(Colour(1,0,0)));
    curNode->Scale(0.3, 0.3, 1);
  } else if (type == 'b') {
    curNode = new Cylinder(new Lambertian(Colour(1,0,0)));
    curNode->Scale(0.3, 0.3, 1);
  } else if (type == 'c') {
    curNode = new Sphere(new Lambertian(Colour(1,0,0)));
    curNode->Scale(0.1, 0.7, 1);
  } else if (type == 'd') {
    curNode = new Sphere(new Lambertian(Colour(1,0,0)));
  }
  curNode->Translate(0, 0, 1);

  double frac = (type == 'a') ? PI/9 : PI/6;
  curTransform = ScaleMatrix(0.8, 0.8, 0.8) * curTransform;
  curTransform = RotateXMatrix(randf(-frac, frac)) * curTransform;
  curTransform = RotateYMatrix(randf(-frac, frac)) * curTransform;
  curTransform = RotateZMatrix(randf(-frac, frac)) * curTransform;
  curTransform = TranslateMatrix(0, 0, 2) * curTransform;
  curTransform = pTransform * curTransform;

  curNode->T = curTransform * curNode->T;
  curNode->finalize();

  obj_list.push_back(curNode);


  char next[25] = "";
  if (type == 'a') strcpy(next, (rand01() < p0) ? "ab" : "bb");
  if (type == 'b') strcpy(next, (rand01() < p1) ? "a" : "c");
  int len = strlen(next);
  for (int i = 0; i < len; i++)
    createLSystem(next[i], depth+1, curTransform, obj_list);
  return;
}

void LSystem::finalize() {
  std::vector<Primitive *> obj_list;
  createLSystem('a', 0, Matrix(), obj_list);
  objs = new AGGREGATE(obj_list);
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