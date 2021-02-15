#ifndef __TRIANGLE_MESH_H__
#define __TRIANGLE_MESH_H__

#include "core/object.h"
#include "util/obj_loader.h"
#include <vector>

struct TriangleMesh : Object {
  TriangleMesh(BSDF *mat) : Object(mat) {}

  TriangleMesh(const char *fname, BSDF *mat, bool bothSides=false) 
    : Object(mat), bothSides(bothSides) { 
    loadObj(fname); 
  };
  
  // Caller should set the `prims` vector to contain all the Triangles.
  void loadTriangles(std::vector<Primitive *>& prims);
  void loadObj(const char *fname);

  bool hit(Ray& r, HitRec &rec);
  Vec sample(double *pdf, RNG& rng) { return T * Vec(); };
  
  Aggregate *mesh;
  bool bothSides = true;
};

#endif // __TRIANGLE_MESH_H__
