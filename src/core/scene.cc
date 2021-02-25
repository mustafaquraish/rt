#include "core/scene.h"
#include "aggregates/bvh.h"
#include "aggregates/primitive_list.h"
#include "objects/triangle_mesh.h"
#include "core/integrator.h"

void Scene::add(Object *obj) { 
  // Finalize object first...
  obj->finalize();
  // Add it to internal list
  obj_list.push_back((Primitive *)obj); 
  // If it's an emitter, add it to light list
  if (obj->bsdf && obj->bsdf->isEmitter())
    lights.push_back(obj);
};

void Scene::finalize() { 
  if (obj_list.size() > 15) 
    world = new BVH(obj_list);
  else 
    world = new PrimitiveList(obj_list);
};

Scene::~Scene() {
  RTMeshList::cleanup();
  delete world;
  delete integrator;
}


Scene *RTSceneFactory::Create(const std::string &name, RenderParams &params) {
  if (mapping.find(name) == mapping.end()) {
    printf("ERROR: Scene %s is not defined.\n", name.c_str());
    exit(1);
  }
  return mapping[name](params);
}

Scene *RTSceneFactory::Create(RenderParams &params) {
  return Create(params.getStr("scene"), params);
}

void RTSceneFactory::Register(const std::string &name, SceneDefinition func) {
  mapping[name] = func;
}
