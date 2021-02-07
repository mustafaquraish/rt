#ifndef __SCENE_H__
#define __SCENE_H__

#include "core/rt.h"
#include "core/object.h"
#include "core/camera.h"
#include "core/integrator.h"
#include <vector>

struct Scene {
  std::vector<Primitive *> obj_list;
  std::vector<Object *> lights;
  Aggregate *world;
  Camera cam;
  int sx;
  int sy;

  void add(Object *obj) { 
    // Finalize object first...
    obj->finalize();
    // Add it to internal list
    obj_list.push_back((Primitive *)obj); 
    // If it's an emitter, add it to light list
    if (obj->bsdf && obj->bsdf->isEmitter())
      lights.push_back(obj);
  };

  void finalize() { 
    world = new AGGREGATE(obj_list);
  };

  bool hit(Ray& ray, HitRec& rec) {
    return world->hit(ray, rec);
  }

  Integrator *integrator;

  // TODO: Complete this
  void buildBVH() { return; }
};



#endif // __SCENE_H__
