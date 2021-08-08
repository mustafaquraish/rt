#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <util/obj_loader.h>
#include <objects/triangle_mesh.h>
#include <core/definitions.h>

#include <aggregates/bvh.h>
#include <aggregates/kdtree.h>
#include <core/definitions.h>

namespace WavefrontOBJ {

/* Quick utility function to check if a string is a prefix of another */
int prefix(const char *pre, const char *str) {
  return strncmp(pre, str, strlen(pre)) == 0;
}

template <MeshType Type>
void load(TriangleMesh<Type> &mesh, const char *filename) {
  if constexpr (Type == MeshType::Simple) {
    auto prims = get_triangles(filename);
    mesh.mesh = new AGGREGATE(prims);
    return;
  }
}

struct MeshData {
  std::vector<Vec3> vs;
  std::vector<Vec3> vns;
  std::vector<Vec3> vts;
  std::vector<Primitive *> faces;
};

void read_faces(const char *buf, MeshData &data) {
  int v0, v1, v2, n0, n1, n2, t0, t1, t2;
  Triangle *t;

  #define VS(i) ((i > 0) ? data.vs[i-1] : data.vs[data.vs.size()+i])
  #define VTS(i) ((i > 0) ? data.vts[i-1] : data.vts[data.vts.size()+i])
  #define VNS(i) ((i > 0) ? data.vns[i-1] : data.vns[data.vns.size()+i])

  // Vertices only
  if (sscanf(buf, "f %d %d %d", &v0, &v1, &v2) == 3) {
    t = new Triangle(VS(v0),  VS(v1),  VS(v2));
  }

  // Vertices + Normals
  if (sscanf(buf, "f %d//%d %d//%d %d//%d", 
             &v0, &n0, &v1, &n1, &v2, &n2) == 6) {
    t = new Triangle(VS(v0),  VS(v1),  VS(v2),
                     VNS(n0), VNS(n1), VNS(n2));
  }
  
  // Vertices + Normals + Textures
  if (sscanf(buf, "f %d/%d/%d %d/%d/%d %d/%d/%d", 
             &v0, &t0, &n0, &v1, &t1, &n1, &v2, &t2, &n2) == 9) {
    t = new Triangle(VS(v0),  VS(v1),  VS(v2),
                     VNS(n0), VNS(n1), VNS(n2),
                     VTS(t0), VTS(t1), VTS(t2));
  }

  #undef VS
  #undef VTS
  #undef VNS

  data.faces.push_back(t);
}

std::vector<Primitive *> get_triangles(const char *filename) {
  FILE *f = fopen(filename, "r");
  if (f == NULL) {
    fprintf(stderr, "Couldn't open %s. Please check name/path.\n", filename);
    exit(1);
  }
  
  char buf[128];
  MeshData data;

  float x, y, z;
  // Store vertices
  while (fgets(buf, 128, f)) {
    if (sscanf(buf, "v %f %f %f", &x, &y, &z))  { data.vs.push_back(Vec3(x, y, z)); }
    if (sscanf(buf, "vt %f %f %f", &x, &y, &z)) { data.vts.push_back(Vec3(x, y, z)); }
    if (sscanf(buf, "vn %f %f %f", &x, &y, &z)) { data.vns.push_back(Vec3(x, y, z)); }
    if (prefix("f ", buf)) { read_faces(buf, data); }
  }
  printf("[+] Loaded %s; vert=%lu, face=%lu\n", filename, data.vs.size(), data.faces.size());
  return data.faces;
}

std::vector<Primitive *> get_submeshes(const char *filename) {
  return std::vector<Primitive *>();
}

template void load(TriangleMesh<Simple> &, const char *);
template void load(TriangleMesh<Full> &, const char *);

}