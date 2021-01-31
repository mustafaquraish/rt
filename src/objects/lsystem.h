#ifndef __LSYSTEM_H__
#define __LSYSTEM_H__

#include "core/rt.h"
#include "core/object.h"
#include <unordered_map>

struct LSystem : Object {
  LSystem(int maxDepth=12, double p0=0.8, double p1=0.8)
    : maxDepth(maxDepth), p0(p0), p1(p1) {};

  // Creates the LSystem objects and stores them in obj_list
  void createLSystem(char type, int depth, Matrix pTransform,
                     std::vector<Primitive *>& obj_list);
  void finalize();

  bool hit(Ray& r, HitRec &rec);
  Vec sample() { return T * Vec(); };

  int maxDepth;
  double p0;
  double p1;

  Aggregate *objs;
};


#endif // __LSYSTEM_H__