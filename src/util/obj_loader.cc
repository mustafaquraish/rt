#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>

#include <aggregates/bvh.h>
#include <aggregates/kdtree.h>
#include <objects/triangle_mesh.h>
#include <core/scene.h>
#include <core/texture.h>
#include <util/obj_loader.h>
#include <util/timer.h>

#include <materials/basic.h>
#include <materials/glossy.h>
#include <materials/emitter.h>

#include <vector>

namespace WavefrontOBJ {

static bool use_mesh_lights = false;
static bool use_glossy_materials = false;
static float mesh_lights_scale = 100;


void set_use_mesh_lights(bool enable) { use_mesh_lights = enable; }
void set_use_glossy_materials(bool enable) { use_glossy_materials = enable; }
void set_mesh_lights_scale(float scale) { mesh_lights_scale = scale; }

struct MeshData {
  MeshData(const char *filepath) {
    m_obj_path = filepath;
    m_obj_base_path = m_obj_path.substr(0, m_obj_path.find_last_of("/\\") + 1);
  }

  struct MeshMaterial {
    Colour Kd = Colour(1);
    Colour Ka = Colour(0);
    Colour Ks = Colour(1);
    Colour Ke = Colour(0);
    Colour Tf = Colour(1);
    int illum = 0;
    float Ns = 0;
    float ref_idx = 1.0;
    float alpha = 1.0;
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
  std::vector<Primitive *> m_lights;

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

  BSDF *get_bsdf_from_material(const MeshMaterial &mat) const;

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

void load(TriangleMesh &mesh, const char *filename) {
  MeshData data(filename);
  if (mesh.m_type == MeshType::Simple) {
    data.read_triangles();
    mesh.loadTriangles(data.m_faces);
    mesh.Scale(-1, 1, 1);

  } else {
    BVH::set_time_build(false);
    data.read_sub_meshes();
    BVH::set_time_build(true);
    mesh.loadTriangles(data.m_sub_meshes);
  }
  if (data.m_lights.size() > 0) {
    mesh.m_lights = data.m_lights;
  }
}

MeshData::Vertex::Vertex(const char *token) {
  // printf("Reading vertex from token: %s\n", token);
  v = t = n = 0;
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
  }
  m_faces.push_back(t);
}

void MeshData::read_faces_from_line(char *line) {
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

  char line[2048];
  while (fgets(line, 2048, f)) {
    char *cur = line;
    while (isspace(*cur)) cur++;
    if (*cur == '\0' || *cur == '#') continue;

    read_basic_from_line(line);
  }
}

void MeshData::read_material_file(const char *filename) {
  if (m_mtl_files.find(filename) != m_mtl_files.end()) return;
  m_mtl_files.insert(filename);

  std::string full_path = m_obj_base_path + filename;

  auto res = m_materials.find(full_path.c_str());
  if (res != m_materials.end()) return;
  
  FILE *f = fopen(full_path.c_str(), "r");
  if (f == NULL) fprintf(stderr, "[-] Can't open \"%s\"\n", full_path.c_str()), exit(1);

  char line[2048];
  char buf[2048];

  MeshMaterial mat;
  std::string material_name;
  float r, g, b;  
  int v;

  auto flush_material = [&](const char *new_name) {
    if (!material_name.empty() || m_materials.find(material_name) != m_materials.end()) {
      m_materials[material_name] = mat;
      mat = MeshMaterial();
    }
    material_name = new_name;
  };

  while (fgets(line, 2048, f)) {
    char *cur = line;
    while (isspace(*cur)) cur++;
    if (*cur == '\0' || *cur == '#') continue;

    if (sscanf(cur, "newmtl %s", buf)) { flush_material(buf); }
    if (sscanf(cur, "Kd %f %f %f", &r, &g, &b)) { mat.Kd = Colour(r, g, b); }
    if (sscanf(cur, "Ka %f %f %f", &r, &g, &b)) { mat.Ka = Colour(r, g, b); }
    if (sscanf(cur, "Ke %f %f %f", &r, &g, &b)) { mat.Ke = Colour(r, g, b); }
    if (sscanf(cur, "Ks %f %f %f", &r, &g, &b)) { mat.Ks = Colour(r, g, b); }
    if (sscanf(cur, "Tf %f %f %f", &r, &g, &b)) { mat.Tf = Colour(r, g, b); }
    if (sscanf(cur, "illum %d", &v)) { mat.illum = v; }
    if (sscanf(cur, "Ns %f", &r)) { mat.Ns = r; }
    if (sscanf(cur, "Ni %f", &r)) { mat.ref_idx = r; }
    if (sscanf(cur, "map_Kd %s", buf)) { 
      mat.texmap = buf; 
      std::replace(mat.texmap.begin(), mat.texmap.end(), '\\', '/');
    }
  }
  flush_material("");
}

BSDF *MeshData::get_bsdf_from_material(const MeshMaterial &mat) const {
  Colour chosen_col = 1;
  BSDF *bsdf = nullptr;
  
  // Emitting objects...
  if (mat.Ke.valid()) {
    if (WavefrontOBJ::use_mesh_lights) {
      chosen_col = mat.Ke * WavefrontOBJ::mesh_lights_scale;
      bsdf = new Emitter(chosen_col);
    } else {
      // Replace light sources with mid-gray if disabled.
      chosen_col = mat.Ke / max(mat.Ke) / 3.0f;
      bsdf = new Lambertian(chosen_col);
    }

  // Refractive objects...
  } else if (mat.illum == 7) {
    chosen_col = mat.Ks;
    if (max(chosen_col) < 0.6) {
      chosen_col *= (0.6 / max(chosen_col));
    }
    float ref_idx = mat.ref_idx == 1 ? 1.5 : mat.ref_idx;
    bsdf = new Transmissive(ref_idx, chosen_col);

  // Reflective objects...
  } else if (mat.illum > 2 && mat.Ns >= 1000) {
    chosen_col = mat.Ks;    
    bsdf = new Mirror(chosen_col); 

  // Diffuse / glossy objects...
  } else {
    
    // Approximate glossy material based on properties
    if (WavefrontOBJ::use_glossy_materials) {

      const float MAX_REFL_SIG = 0.3;
      
      Colour col = mat.Ka + mat.Kd + mat.Ks;
      float diffuse = length(mat.Ka) + length(mat.Kd);
      
      diffuse = diffuse / length(col);
      float reflect = 1 - diffuse;
      
      float refl_sig = 0;
      if (mat.Ks.valid() && mat.Ns > 0 && mat.Ns <= 999)
        refl_sig = MAX_REFL_SIG - (MAX_REFL_SIG * (mat.Ns/1000.0));

      float max_col = max(col);
      if (max_col > 1)
        col /= max_col;

      chosen_col = col;
      bsdf = new Glossy(chosen_col, diffuse, refl_sig);
    
    // Use a lambertian material
    } else {
      chosen_col = mat.Ka + mat.Kd;
      bsdf = new Lambertian(chosen_col);
    }
  }

  if (!mat.texmap.empty()) {
    std::string full_path = m_obj_base_path + mat.texmap;
    if (chosen_col.valid()) {
      bsdf->m_tx = new TintedImageTexture(full_path.c_str(), chosen_col);
    } else {
      bsdf->m_tx = new ImageTexture(full_path.c_str());
    }
  }

  return bsdf;
}

void MeshData::build_sub_mesh() {
  if (m_faces.empty() || m_cur_material_name.empty()) return;
  
  auto res = m_materials.find(m_cur_material_name);
  if (res == m_materials.end()) { fprintf(stderr, "Error, couldn't find mat: %s\n", m_cur_material_name.c_str()), exit(1); }
  
  MeshMaterial &mat = res->second;
  BSDF *bsdf = get_bsdf_from_material(mat);
  
  auto *mesh = new TriangleMesh(bsdf);
  mesh->loadTriangles(m_faces);
  mesh->Scale(-1, 1, 1);

  if (mesh->bsdf->isEmitter()) {
    m_lights.push_back(mesh);
  } else {
    mesh->finalize();
    m_sub_meshes.push_back(mesh);
  }

  m_faces.clear();
}

void MeshData::read_sub_meshes() {
  Timer timer("Loading '%s' (full)", m_obj_path.c_str());

  FILE *f = fopen(m_obj_path.c_str(), "r");
  if (f == NULL) fprintf(stderr, "[-] Can't open \"%s\"\n", m_obj_path.c_str()), exit(1);

  char line[2048];
  char buf[2048];
  while (fgets(line, 2048, f)) {
    char *cur = line;
    while (isspace(*cur)) cur++;
    if (*cur == '\0' || *cur == '#') continue;

    read_basic_from_line(cur);
    if (sscanf(cur, "mtllib %s", buf)) { read_material_file(buf); }
    if (sscanf(cur, "usemtl %s", buf)) { build_sub_mesh(), m_cur_material_name = buf; }
    // if (sscanf(cur, "g %s", buf)) { m_cur_material_name = buf; }
    // if (sscanf(cur, "o %s", buf)) { build_sub_mesh(); }
  }
  build_sub_mesh();
  printf("[+] Loaded %d sub meshes\n", (int)m_sub_meshes.size());
}

}