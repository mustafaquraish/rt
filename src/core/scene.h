#ifndef __SCENE_H__
#define __SCENE_H__

#include "core/object.h"
#include "core/camera.h"
#include "core/integrator.h"
#include <vector>

struct Scene {
  std::vector<Primitive *> obj_list;
  Aggregate *world;
  // PrimitiveList als_list;
  Camera cam;
  int sx;
  int sy;
  // std::vector<PointLS> pls_list;

  void add(Object *obj) { 
    // Finalize object first...
    obj->finalize();
    // Add it to internal list
    obj_list.push_back((Primitive *)obj); 
  };

  Integrator *integrator;

  // TODO: Complete this
  void buildBVH() { return; }
};



#endif // __SCENE_H__
