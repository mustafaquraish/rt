#include <objects/csg.h>

void CSG::set_rec(HitRec &t, HitRec &rec) {
  if (t.t < rec.t) {
    rec = t;
  }
}

bool CSG::hit_union(Ray &ray, CSGData &data, HitRec &rec) { 
  // if (!data.hit[0] && !data.hit[1]) return false;
  // if (!data.hit[0]) { *rec = r[1]; rec->obj = obj; return true; }
  // if (!data.hit[1]) { *rec = r[0]; rec->obj = obj; return true; }

  // rec->obj = obj;

  // if (a1 <= b1) {
  //   set_rec(0, a1, na1);
  //   if      (a2 < b1) set_rec(1, a2, na2);
  //   else if (a2 > b2) set_rec(1, a2, na2);
  //   else              set_rec(1, b2, nb2);

  // } else {
  //   set_rec(0, b1, nb1);
  //   if      (b2 < a1) set_rec(1, b2, nb2);
  //   else if (b2 > a2) set_rec(1, b2, nb2);
  //   else              set_rec(1, a2, na2);
  // }

}


bool CSG::hit_difference(Ray &ray, CSGData &data, HitRec &rec) { return false; }


bool CSG::hit_intersection(Ray &ray, CSGData &data, HitRec &rec) {
  if (!data.hit[0] || !data.hit[1]) return false;
  if ((data.a1.t < data.b1.t && data.a2.t < data.b1.t) ||
      (data.b1.t < data.a1.t && data.b2.t < data.a1.t)) {
    return false;
  }
  if (data.a1.t < data.b1.t) {
    set_rec(data.b1, rec);
    if (data.a2.t < data.b2.t) set_rec(data.a2, rec);
    else                       set_rec(data.b2, rec);

  } else {
    set_rec(data.a1, rec);
    if (data.b2.t < data.a2.t) set_rec(data.b2, rec);
    else         set_rec(data.a2, rec);
  }
  return true;
}

bool CSG::hit(Ray &r, HitRec &rec) {
  Ray transformed = rayTransform(r);

  CSGData data;
  data.a1.t = -1;
  data.a2.t = -1;
  data.b1.t = -1;
  data.b2.t = -1;

  if (m_obj_a->hit(transformed, data.a1)) {
    if (dot(data.a1.n, transformed.d) < 0) {
      Ray temp_ray = Ray(transformed.at(data.a1.t), transformed.d);
      m_obj_a->hit(temp_ray, data.a2);
      data.a2.t += data.a1.t;
    } else {
      std::swap(data.a1, data.a2);
    }
    data.hit[0] = true;
  }

  if (m_obj_b->hit(transformed, data.b1)) {
    if (dot(data.b1.n, transformed.d) < 0) {
      Ray temp_ray = Ray(transformed.at(data.b1.t), transformed.d);
      m_obj_b->hit(temp_ray, data.b2);
      data.b2.t += data.b1.t;
    } else {
      std::swap(data.b1, data.b2);
    }
    data.hit[1] = true;
  }

  switch (m_type) {
    case Union:
      return hit_union(r, data, rec);
    case Intersection:
      return hit_intersection(r, data, rec);
    case Difference:
      return hit_difference(r, data, rec);
  }

  // Shouldn't get here
  return false;
}