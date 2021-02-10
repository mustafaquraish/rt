#ifndef __SCENE_H__
#define __SCENE_H__

#include "core/camera.h"
#include "core/integrator.h"
#include "core/object.h"
#include "core/rt.h"

#include <unordered_map>
#include <vector>

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

typedef Scene *(*SceneDefinition)(RenderParams &params);

struct SceneFactory {
  inline static std::unordered_map<const char *, SceneDefinition> mapping;

  static Scene *Create(const char *name, RenderParams &params);
  static void Register(const char *name, SceneDefinition func);
};

// Define and register a scene
#define SCENE(name)                                                            \
  Scene *RT_Scene_##name(RenderParams &params);                                \
  static struct RT_Scene_constructor_##name {                                  \
    RT_Scene_constructor_##name() {                                            \
      SceneFactory::Register(#name, RT_Scene_##name);                          \
    };                                                                         \
  } RT_Scene_instance_##name;                                                  \
  Scene *RT_Scene_##name(RenderParams &params)

#endif // __SCENE_H__
