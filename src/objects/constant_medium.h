#pragma once

#include <core/object.h>

struct ConstantMedium : Object {
  ConstantMedium(float density, Object *obj) 
    : Object(nullptr), m_obj(obj), m_density(density) { };
  bool hit(Ray &r, HitRec &rec) override;

  void finalize() override {
    m_obj->finalize();
    this->bounds = m_obj->bounds;
    Object::finalize();
  };

private:
  float m_density;
  Object *m_obj;
};
