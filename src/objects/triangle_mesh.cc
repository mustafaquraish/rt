#include <util/obj_loader.h>
#include <objects/triangle_mesh.h>

#include <aggregates/bvh.h>
#include <aggregates/kdtree.h>
#include <core/definitions.h>

void RTMeshList::registerMesh(Aggregate *mesh) {
  allMeshes.insert(mesh);
}

void RTMeshList::registerObj(std::string filename, Aggregate *mesh) {
  objFileMapping[filename] = mesh;
  allMeshes.insert(mesh);
}

Aggregate *RTMeshList::getObjMesh(std::string filename) {
  if (objFileMapping.find(filename) == objFileMapping.end())
    return NULL;
  return objFileMapping[filename];
}

void RTMeshList::cleanup() {
  for (auto agg : allMeshes) {
    delete agg;
  }
  allMeshes.clear();
  objFileMapping.clear();
}

/*****************************************************************************/

TriangleMesh::TriangleMesh(BSDF *mat) : Object(mat) {}

TriangleMesh::TriangleMesh(const char *fname, BSDF *mat, bool bothSides) 
    : Object(mat), bothSides(bothSides) { 

  mesh = RTMeshList::getObjMesh(fname);
  if (mesh == NULL) {
    std::vector<Primitive *> prims_list = wavefrontObjLoader(fname);
    loadTriangles(prims_list);
    RTMeshList::registerObj(fname, mesh);
  }
};

// Caller should set the `prims` vector to contain all the Triangles.
void TriangleMesh::loadTriangles(std::vector<Primitive *>& prims) {
  // mesh = new KDTree(prims);
  // mesh = new BVH(prims);
  mesh = new AGGREGATE(prims);
  bounds = mesh->bounds;
  RTMeshList::registerMesh(mesh);
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