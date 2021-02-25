#ifndef __SCENE_H__
#define __SCENE_H__

#include "core/camera.h"
#include "core/object.h"

#include <unordered_map>
#include <vector>

struct Integrator;

struct Scene {
  std::vector<Primitive *> obj_list;
  std::vector<Object *> lights;
  Aggregate *world;

  Camera cam;
  int sx;
  int sy;

  Integrator *integrator;

  void add(Object *obj);
  void finalize();

  // In header so compiler can inline it easier; performance critical.
  bool hit(Ray &ray, HitRec &rec) { return world->hit(ray, rec); }
};


struct RTSceneFactory {
  typedef Scene *(*SceneDefinition)(RenderParams &params);
  inline static std::unordered_map<std::string , SceneDefinition> mapping;

  static Scene *Create(RenderParams &params);
  static Scene *Create(const std::string &name, RenderParams &params);
  static void Register(const std::string &name, SceneDefinition func);
};

// Define and register a scene
#define SCENE(name)                                                           \
  Scene *RT_Scene_##name(RenderParams &params);                               \
  static struct RT_Scene_constructor_##name {                                 \
    RT_Scene_constructor_##name() {                                           \
      RTSceneFactory::Register(#name, RT_Scene_##name);                       \
    };                                                                        \
  } RT_Scene_instance_##name;                                                 \
  Scene *RT_Scene_##name(RenderParams &params)

#endif // __SCENE_H__
