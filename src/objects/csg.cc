#include <objects/csg.h>

namespace CSG {

struct CSGRange {
  HitRec min, max;
  bool hit;
};

bool get_both_intersections(Object *obj, Ray& ray, HitRec& r1, HitRec& r2) {
  Ray temp_ray = Ray(ray.p, ray.d);
  if (obj->hit(temp_ray, r1)) {
    if (dot(r1.n, ray.d) < 0) {
      temp_ray = Ray(ray.at(r1.t), ray.d);
      obj->hit(temp_ray, r2);
      r2.t += r1.t;
    } else {
      std::swap(r1, r2);
      r1.t = -1;
    }
    return true;
  }
  return false;
}

void set_correct_rec(Object *o, HitRec &r1, HitRec &r2, HitRec &actual) {
  actual = r1.t < 0 ? r2 : r1;
  actual.obj = o;
}

bool Union(CSGRange &a, CSGRange &b, CSGRange &res) { 
  if (!a.hit && !b.hit) return false;
  if (!a.hit) { res = b; return true; }
  if (!b.hit) { res = a; return true; }

  if (a.min.t <= b.min.t) {
    res.min = a.min;
    if      (a.max.t < b.min.t) res.max = a.max;
    else if (a.max.t > b.max.t) res.max = a.max;
    else                        res.max = b.max;

  } else {
    res.min = b.min;
    if      (b.max.t < a.min.t) res.max = b.max;
    else if (b.max.t > a.max.t) res.max = b.max;
    else                        res.max = a.max;
  }
  return true;
}


bool Difference(CSGRange &a, CSGRange &b, CSGRange &res) { 
  if (!a.hit) return false;
  if (a.hit && !b.hit) { res = a; return true; }
  if (b.min.t < a.min.t && a.max.t < b.max.t) return false;

  if (a.min.t <= b.min.t) {
    res.min = a.min;
    if      (a.max.t < b.min.t) res.max = a.max;
    else if (a.max.t < b.max.t) {
      res.max = b.min;
      res.max.n = -res.max.n;
    }

    // Need to see where the light ray starts, so we give return
    // the appropriate component
    else if (b.min.t > TOL) {
      res.max = b.min;
      res.max.n = -res.max.n;

    } else if (b.max.t > TOL) {
      res.min = b.max;
      res.min.n = -res.min.n;

      res.max = a.max;
    }

  // CAREFUL: not symmetric
  } else {
    if      (b.max.t < a.min.t) res.min = a.min, res.max = a.max;
    else if (b.max.t < a.max.t) {
      res.min = b.max;
      res.min.n = -res.min.n;
      
      res.max = a.max;
    }
  }
  return true;
}


bool Intersection(CSGRange &a, CSGRange &b, CSGRange &res) {
  if (!a.hit || !b.hit) return false;

  if ((a.min.t < b.min.t && a.max.t < b.min.t) || 
      (b.min.t < a.min.t && b.max.t < a.min.t)) { 
    return false; 
  } 

  if (a.min.t <= b.min.t) {
    res.min = b.min;
    if (a.max.t <= b.max.t) res.max = a.max;
    else                    res.max = b.max;
  } else {
    res.min = a.min;
    if (b.max.t <= a.max.t) res.max = b.max;
    else                    res.max = a.max;
  }
  return true;
}

}

bool CSGObject::hit(Ray &r, HitRec &rec) {
  Ray transformed = rayTransform(r);
  
  CSG::CSGRange a, b, res; 
  a.hit = CSG::get_both_intersections(m_obj_a, transformed, a.min, a.max);
  b.hit = CSG::get_both_intersections(m_obj_b, transformed, b.min, b.max);

  switch (m_type) {
    case Union:        res.hit = CSG::Union(a, b, res); break;
    case Intersection: res.hit = CSG::Intersection(a, b, res); break;
    case Difference:   res.hit = CSG::Difference(a, b, res); break;
  }

  if (res.hit) {
    rec = (res.min.t > TOL) ? res.min : res.max;
    rec.p = r.at(rec.t);
    rec.n = normalTransform(rec.n);
    rec.obj = this;
    return true;
  }

  return false;
}