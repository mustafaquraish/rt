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

template <MeshType Type>
TriangleMesh<Type>::TriangleMesh(BSDF *mat) : Object(mat) {}

template <MeshType Type>
TriangleMesh<Type>::TriangleMesh(const char *fname, BSDF *mat, bool bothSides) 
    : Object(mat), bothSides(bothSides) { 

  mesh = RTMeshList::getObjMesh(fname);
  if (mesh == NULL) {
    WavefrontOBJ::load(*this, fname);
    RTMeshList::registerObj(fname, mesh);
  }
};

// Caller should set the `prims` vector to contain all the Triangles.
template <MeshType Type>
void TriangleMesh<Type>::loadTriangles(std::vector<Primitive *>& prims) {
  mesh = new AGGREGATE(prims);
  bounds = mesh->bounds;
  RTMeshList::registerMesh(mesh);
}

template <MeshType Type>
bool TriangleMesh<Type>::hit(Ray& r, HitRec &rec) {
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

// Explicit template instantiation
template class TriangleMesh<Simple>;
template class TriangleMesh<Full>;