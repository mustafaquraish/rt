#include "objects/triangle_mesh.h"

  // Caller should set the `prims` vector to contain all the Triangles.
void TriangleMesh::loadTriangles() {
  mesh = new BVH(prims);
  // mesh = new PrimitiveList(prims);
  bounds = mesh->getBounds();
}

void TriangleMesh::loadObj(const char *fname) {
  prims = wavefrontObjLoader(fname);
  loadTriangles();
}


bool TriangleMesh::hit(Ray& r, HitRec &rec) {
  Ray transformed = rayTransform(r);
  if (!mesh) {
    printf("Error: `mesh` not set for TriangleMesh.\n");
    exit(1);
  }
  if (!mesh->hit(transformed, rec)) {
    return false;
  }

  rec.p = r.at(rec.t1);
  rec.n = normalTransform(rec.n);
  return true;
}