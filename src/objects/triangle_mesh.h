#ifndef __TRIANGLE_MESH_H__
#define __TRIANGLE_MESH_H__

#include "core/object.h"
#include "util/obj_loader.h"
#include <vector>

struct TriangleMesh : Object {
  TriangleMesh(Material *mat) : Object(mat) {}

  TriangleMesh(const char *fname, Material *mat) : Object(mat) { 
    loadObj(fname); 
  };

  // Caller should set the `prims` vector to contain all the Triangles.
  void loadTriangles(std::vector<Primitive *>& prims);
  void loadObj(const char *fname);

  bool hit(Ray& r, HitRec &rec);
  Vec sample() { return T * Vec(); };
  
  Aggregate *mesh;
};

#endif // __TRIANGLE_MESH_H__
