#pragma once

#include <core/object.h>

enum CSGType {
  Union,
  Intersection,
  Difference,
};

struct CSGObject : Object {
  CSGObject(CSGType type, Object *a, Object *b)
    : m_type(type), m_obj_a(a), m_obj_b(b) { };

  virtual void finalize() override {
    m_obj_a->finalize();
    m_obj_b->finalize();
    this->bounds = combine(m_obj_a->bounds, m_obj_b->bounds);
    Object::finalize();
  }

  virtual bool hit(Ray &r, HitRec &rec) override;

private:

  CSGType m_type;
  Object *m_obj_a;
  Object *m_obj_b;
};
