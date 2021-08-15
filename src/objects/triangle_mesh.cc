#include <util/obj_loader.h>
#include <objects/triangle_mesh.h>

#include <aggregates/bvh.h>
#include <aggregates/kdtree.h>
#include <aggregates/primitive_list.h>

#include <materials/lambertian.h>

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

TriangleMesh::TriangleMesh(BSDF *mat) : Object(mat), m_type(MeshType::Simple) {}

TriangleMesh::TriangleMesh(MeshType type, const char *fname, BSDF *mat, bool bothSides) 
    : Object(mat), bothSides(bothSides), m_type(type) { 
  
  if (m_type == MeshType::Simple) {
    if (mat == nullptr) {
      fprintf(stderr, "Error: No material specified for simple mesh, assigning default...\n");
      setMaterial(new Lambertian(1));
    }
  } 

  mesh = RTMeshList::getObjMesh(fname);
  if (mesh == NULL) {
    WavefrontOBJ::load(*this, fname);
    RTMeshList::registerObj(fname, mesh);
  }
};

// Caller should set the `prims` vector to contain all the Triangles.
void TriangleMesh::loadTriangles(std::vector<Primitive *>& prims) {
  
  if (prims.size() < 10) {
    mesh = new PrimitiveList(prims);
  } else {
    mesh = new BVH(prims);
  }
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
  r.tMax = min(r.tMax, rec.t);
  if (bothSides && dot(rec.n, r.d) > 0) rec.n = -rec.n;

  if (m_type == Simple) {
    rec.obj = this;
  }

  return true;
}
