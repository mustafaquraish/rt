#pragma once

#include <core/object.h>

struct CSG : Object {
  enum CSGType {
    Union,
    Intersection,
    Difference,
  };

  CSG(CSGType type, Object *a, Object *b)
    : m_type(type), m_obj_a(a), m_obj_b(b) { };

  virtual void finalize() override {
    m_obj_a->finalize();
    m_obj_b->finalize();
    Object::finalize();
  }

  virtual bool hit(Ray &r, HitRec &rec) override;


private:
  struct CSGData {
    HitRec a1, a2, b1, b2;
    bool hit[2];
  };

  void set_rec(HitRec &t, HitRec &rec);

  bool hit_union(Ray &, CSGData &, HitRec &);
  bool hit_intersection(Ray &, CSGData &, HitRec &);
  bool hit_difference(Ray &, CSGData &, HitRec &);

  CSGType m_type;
  Object *m_obj_a;
  Object *m_obj_b;
};
