#include "core/scene.h"

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
  world = new AGGREGATE(obj_list);
};

Scene *SceneFactory::Create(const std::string &name, RenderParams &params) {
  if (mapping.find(name) == mapping.end()) {
    printf("ERROR: Scene %s is not defined.\n", name.c_str());
    exit(1);
  }
  return mapping[name](params);
}

Scene *SceneFactory::Create(RenderParams &params) {
  return Create(params.getStr("scene"), params);
}

void SceneFactory::Register(const std::string &name, SceneDefinition func) {
  mapping[name] = func;
}