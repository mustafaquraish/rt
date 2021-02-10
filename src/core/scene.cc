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

Scene *SceneFactory::Create(const char *name, RenderParams &params) {
  if (SceneFactory::mapping.find(name) == SceneFactory::mapping.end()) {
    printf("ERROR: Scene %s is not defined.\n", name);
    exit(1);
  }
  return SceneFactory::mapping[name](params);
}

void SceneFactory::Register(const char *name, SceneDefinition func) {
  SceneFactory::mapping[name] = func;
}
