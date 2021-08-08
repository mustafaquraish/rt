#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>

#include <aggregates/bvh.h>
#include <aggregates/kdtree.h>
#include <materials/lambertian.h>
#include <objects/triangle_mesh.h>
#include <core/texture.h>
#include <util/obj_loader.h>
#include <util/timer.h>

namespace WavefrontOBJ {

struct MeshData {
  MeshData(const char *filepath) {
    m_obj_path = filepath;
    m_obj_base_path = m_obj_path.substr(0, m_obj_path.find_last_of("/\\") + 1);
    std::cout << "obj path: " << m_obj_path << std::endl;
    std::cout << "obj base path: " << m_obj_base_path << std::endl;
  }

  struct MeshMaterial {
    Colour col;
    std::string texmap;
  };
  
  // Directory and name of the OBJ file
  std::string m_obj_path;
  std::string m_obj_base_path;

  std::vector<Vec3> m_vs;
  std::vector<Vec3> m_vns;
  std::vector<Vec2> m_vts;
  std::vector<Primitive *> m_faces;

  std::vector<Primitive *> m_sub_meshes;

  std::unordered_set<std::string> m_mtl_files;
  std::unordered_map<std::string, MeshMaterial> m_materials;
  std::string m_cur_material_name;

  std::string m_sub_mesh_name;


  Vec3 vs(int i) const { return (i > 0) ? m_vs[i-1] : m_vs[m_vs.size()+i]; }
  Vec3 vns(int i) const { return (i > 0) ? m_vns[i-1] : m_vns[m_vns.size()+i]; }
  Vec2 vts(int i) const { return (i > 0) ? m_vts[i-1] : m_vts[m_vts.size()+i]; }

  void read_material_file(const char *);
  void build_sub_mesh();

  struct Vertex {
    Vertex() {}
    enum VertexType {
      Pos,
      PosNormal,
      PosTexNormal,
    };
    Vertex(const char *token);
    int v, n, t;
    VertexType type;
  };

  void make_triangle_from_points(Vertex &, Vertex &, Vertex &);
  void read_faces_from_line(char *line);
  void read_sub_meshes();
  void read_triangles();

  void read_basic_from_line(char *line);
};

/* Quick utility function to check if a string is a prefix of another */
int prefix(const char *pre, const char *str) {
  return strncmp(pre, str, strlen(pre)) == 0;
}

template <MeshType Type>
void load(TriangleMesh<Type> &mesh, const char *filename) {
  MeshData data(filename);
  if constexpr (Type == MeshType::Simple) {
    data.read_triangles();
    mesh.mesh = new BVH(data.m_faces);
  } else {
    BVH::set_time_build(false);
    data.read_sub_meshes();
    BVH::set_time_build(true);
    mesh.mesh = new BVH(data.m_sub_meshes);
  }
}

MeshData::Vertex::Vertex(const char *token) {
  // printf("Reading vertex from token: %s\n", token);
  if      (sscanf(token, "%d/%d/%d", &v, &t, &n) == 3) { type = PosTexNormal; }
  else if (sscanf(token, "%d//%d", &v, &n) == 2) { type = PosNormal; }
  else if (sscanf(token, "%d", &v) == 1) { type = Pos; }
}

void MeshData::make_triangle_from_points(Vertex &v0, Vertex &v1, Vertex &v2) {
  // Vertices only
  Triangle *t;
  
  if (v0.type == Vertex::Pos) {
    t = new Triangle(vs(v0.v),  vs(v1.v),  vs(v2.v));
  }

  // Vertices + Normals
  else if (v0.type == Vertex::PosNormal) {
    t = new Triangle(vs(v0.v),  vs(v1.v),  vs(v2.v),
                     vns(v0.n), vns(v1.n), vns(v2.n));
  }
  
  // Vertices + Normals + Textures
  else if (v0.type == Vertex::PosTexNormal) {
    t = new Triangle(vs(v0.v),  vs(v1.v),  vs(v2.v),
                     vns(v0.n), vns(v1.n), vns(v2.n),
                     vts(v0.t), vts(v1.t), vts(v2.t));
    // printf("Triangle has tex: (%f %f), (%f %f), (%f %f)\n",
    //        vts(v0.t).u, 
    //        vts(v0.t).v,
    //        vts(v1.t).u,
    //        vts(v1.t).v,
    //        vts(v2.t).u,
    //        vts(v2.t).v);
  }
  m_faces.push_back(t);
}

void MeshData::read_faces_from_line(char *line) {
  // printf("-- Reading faces from line: ''%s''\n", line);

  Vertex v0, v1, v2;
  Triangle *t;

  line += 2; // Skip over "f "

  char *token;
  v0 = Vertex(strsep(&line, " "));
  v1 = Vertex(strsep(&line, " "));

  while( (token = strsep(&line," ")) != NULL ) {
    v2 = Vertex(token);
    make_triangle_from_points(v0, v1, v2);
    v1 = v2;
  }
}

void MeshData::read_basic_from_line(char *line) {
  float x, y, z;
  if (sscanf(line, "v %f %f %f", &x, &y, &z))  { m_vs.push_back(Vec3(x, y, z)); }
  if (sscanf(line, "vn %f %f %f", &x, &y, &z)) { m_vns.push_back(Vec3(x, y, z)); }
  if (sscanf(line, "vt %f %f", &x, &y))        { m_vts.push_back(Vec2(x, y)); }
  if (prefix("f ", line)) { read_faces_from_line(line); }
}

void MeshData::read_triangles() {
  Timer timer("Loading '%s'", m_obj_path.c_str());

  FILE *f = fopen(m_obj_path.c_str(), "r");
  if (f == NULL) fprintf(stderr, "[-] Can't open \"%s\"\n", m_obj_path.c_str()), exit(1);

  char line[128];
  while (fgets(line, 128, f)) { read_basic_from_line(line); }
}

void MeshData::read_material_file(const char *filename) {
  if (m_mtl_files.find(filename) != m_mtl_files.end()) return;
  m_mtl_files.insert(filename);

  std::string full_path = m_obj_base_path + filename;

  auto res = m_materials.find(full_path.c_str());
  if (res != m_materials.end()) return;
  
  FILE *f = fopen(full_path.c_str(), "r");
  if (f == NULL) fprintf(stderr, "[-] Can't open \"%s\"\n", full_path.c_str()), exit(1);

  char line[128];
  char buf[128];

  MeshMaterial mat;
  std::string material_name;
  float r, g, b;  

  auto flush_material = [&](const char *new_name) {
    if (!material_name.empty() || m_materials.find(material_name) != m_materials.end()) {
      // std::cout << "Adding new material ("<< material_name << ") with col: " << mat.col << " an tex: ("<<mat.texmap<<")\n";
      m_materials[material_name] = mat;
      mat.texmap.clear();
      mat.col = {0, 1, 0};
    }
    material_name = new_name;
  };

  while (fgets(line, 128, f)) { 
    if (sscanf(line, "newmtl %s", buf)) { flush_material(buf); }
    if (sscanf(line, "Kd %f %f %f", &r, &g, &b)) { mat.col = Colour(r, g, b); }
    if (sscanf(line, "map_Kd %s", buf)) { 
      mat.texmap = buf; 
      std::replace(mat.texmap.begin(), mat.texmap.end(), '\\', '/');
    }
  }
  flush_material("");
}

void MeshData::build_sub_mesh() {
  if (m_faces.empty() || m_cur_material_name.empty()) return;
  
  auto res = m_materials.find(m_cur_material_name);
  if (res == m_materials.end()) { fprintf(stderr, "Error, couldn't find mat: %s\n", m_cur_material_name.c_str()), exit(1); }
  MeshMaterial &mat = res->second;

  BSDF *bsdf = nullptr;
  if (mat.texmap.empty()) {
    bsdf = new Lambertian(mat.col);
  } else {
    std::string full_path = m_obj_base_path + mat.texmap;
    bsdf = new Lambertian(new ImageTexture(full_path.c_str()));
  }
  
  auto *mesh = new TriangleMesh<Simple>(bsdf);
  mesh->loadTriangles(m_faces);
  m_sub_meshes.push_back(mesh);

  m_faces.clear();
}

void MeshData::read_sub_meshes() {
  Timer timer("Loading '%s' (full)", m_obj_path.c_str());

  FILE *f = fopen(m_obj_path.c_str(), "r");
  if (f == NULL) fprintf(stderr, "[-] Can't open \"%s\"\n", m_obj_path.c_str()), exit(1);

  char line[128];
  char buf[128];
  while (fgets(line, 128, f)) { 
    read_basic_from_line(line); 
    if (sscanf(line, "mtllib %s", buf)) { read_material_file(buf); }
    if (sscanf(line, "usemtl %s", buf)) { build_sub_mesh(), m_cur_material_name = buf; }
    // if (sscanf(line, "g %s", buf)) { m_cur_material_name = buf; }
    if (sscanf(line, "o %s", buf)) { build_sub_mesh(); }
  }
  build_sub_mesh();
  printf("[+] Loaded %d sub meshes\n", (int)m_sub_meshes.size());
}

template void load(TriangleMesh<Simple> &, const char *);
template void load(TriangleMesh<Full> &, const char *);

}