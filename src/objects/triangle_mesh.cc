#include <aggregates/bvh.h>
#include <aggregates/primitive_list.h>
#include <core/rng.h>
#include <core/scene.h>
#include <materials/basic.h>
#include <objects/triangle_mesh.h>
#include <util/obj_loader.h>

#include <cassert>

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
  m_prims = &mesh->m_prims;
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

Vec3 TriangleMesh::sample(float *pdf, RNG& rng) {
  auto &prims = *m_prims;
  
  assert(prims.size() > 0);
  assert(m_type == Simple);
  
  // printf("...?\n");

  int idx = rng.randint(0, prims.size() - 1);
  Primitive *prim = prims[idx];
  Triangle *tri = static_cast<Triangle *>(prim);


  float a = rng.rand01(), b = rng.rand01();
  float sqa = sqrt(a), sqb = sqrt(b);

  Vec3 pt = (1-sqa)*tri->p[0] + (sqa*(1-b))*tri->p[1] + (b*sqa)*tri->p[2];
  float sf = 10;
  *pdf = 1 / (sf * prims.size());
  return T * pt;
}

void TriangleMesh::finalize() {
  if (m_lights.size() > 0) {
    assert(m_type == Full);
    for (Primitive *light : m_lights) {
      TriangleMesh *m = static_cast<TriangleMesh *>(light);
      m->T *= T;
      CurrentSceneGlobal->add(m);
    }
  }

  Object::finalize();
}