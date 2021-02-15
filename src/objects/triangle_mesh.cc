#include "core/rt.h"
#include "objects/triangle_mesh.h"

// Caller should set the `prims` vector to contain all the Triangles.
void TriangleMesh::loadTriangles(std::vector<Primitive *>& prims) {
  mesh = new AGGREGATE(prims);
  bounds = mesh->bounds;
}

void TriangleMesh::loadObj(const char *fname) {
  mesh = wavefrontObjLoader(fname);
  bounds = mesh->bounds;
}


bool TriangleMesh::hit(Ray& r, HitRec &rec) {
  Ray transformed = rayTransform(r);
  if (!mesh) {
    printf("Error: `mesh` not set for TriangleMesh.\n");
    exit(1);
  }
  if (!mesh->hit(transformed, rec)) return false;

  rec.p = r.at(rec.t);
  rec.n = normalTransform( normalMapped(rec.n, rec) );
  rec.obj = this;
  r.tMax = min(r.tMax, rec.t);

  if (bothSides && dot(rec.n, r.d) > 0) rec.n = -rec.n;

  return true;
}