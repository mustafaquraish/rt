#pragma once

#include <core/object.h>
#include <objects/triangle.h>
#include <vector>
#include <unordered_map>
#include <unordered_set>

struct RTMeshList {
  inline static std::unordered_map<std::string, Aggregate *> objFileMapping;
  inline static std::unordered_set<Aggregate *> allMeshes;

  static void registerMesh(Aggregate *mesh);
  static void registerObj(std::string filename, Aggregate *mesh);
  static Aggregate *getObjMesh(std::string filename);

  static void cleanup();
};

enum MeshType {
  Simple,    // Load only the geometry of the mesh 
  Full,      // Load the geometry and materials, create sub-meshes
};

struct TriangleMesh : Object {
  TriangleMesh(BSDF *mat);
  
  TriangleMesh(MeshType type, const char *fname, BSDF *mat=nullptr, bool bothSides=true);

  // Caller should set the `prims` vector to contain all the Triangles.
  void loadTriangles(std::vector<Primitive *>& prims);

  bool hit(Ray& r, HitRec &rec);
  Vec3 sample(float *pdf, RNG& rng);
  void finalize();

  std::vector<Primitive *> *m_prims = nullptr;
  std::vector<Primitive *> m_lights;
  Aggregate *mesh;
  bool bothSides = false;
  MeshType m_type;
};


struct RescaledMesh : TriangleMesh {
  using TriangleMesh::TriangleMesh;

  virtual void finalize() override {
    float scale = max(range(this->mesh->bounds));
    this->T *= ScaleMatrix(1/scale, 1/scale, 1/scale);
    
    Vec3 pt = -centroid(this->mesh->bounds);
    this->T *= TranslateMatrix(pt.x, pt.y, pt.z);
 
    TriangleMesh::finalize();
  }
};

