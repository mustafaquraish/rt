#ifndef __LSYSTEM_H__
#define __LSYSTEM_H__

#include "core/object.h"

struct LSystem : Object {
  LSystem(int seed) : seed(seed) {};
  LSystem(int seed, int maxDepth) : seed(seed), maxDepth(maxDepth) {};

  // Creates the LSystem objects and stores them in obj_list
  void createLSystem(char type, int depth, Matrix pTransform,
                     std::vector<Primitive *>& obj_list, RNG& rng);
  void finalize();

  bool hit(Ray& r, HitRec &rec);
  Vec sample(double *pdf, RNG& rng) { return T * Vec(); };

  int maxDepth = 12;
  double p0 = 0.8;
  double p1 = 0.8;

  int seed = rand();
  
  Aggregate *objs;
};


#endif // __LSYSTEM_H__
