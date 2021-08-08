#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <util/obj_loader.h>
#include <objects/triangle.h>


/* Quick utility function to check if a string is a prefix of another */
int prefix(const char *pre, const char *str) {
  return strncmp(pre, str, strlen(pre)) == 0;
}

std::vector<Primitive *> wavefrontObjLoader(const char *fname) {
  FILE *f = fopen(fname, "r");
  if (f == NULL) {
    fprintf(stderr, "Couldn't open %s. Please check name/path.\n", fname);
    exit(1);
  }
  
  char buf[128];
  std::vector<Vec3> vs;
  std::vector<Vec3> vns;
  std::vector<Vec3> vts;
  std::vector<Primitive *> faces;

  float x, y, z;
  // Store vertices
  while (fgets(buf, 128, f)) {
    if (prefix("v ", buf)) {
      sscanf(buf, "v %f %f %f", &x, &y, &z);
      // std::cout << "Vertex: " << Vec3(x,y,z) << std::endl;
      vs.push_back(Vec3(x, y, z));
    }
    if (prefix("vt ", buf)) {
      sscanf(buf, "vt %f %f", &x, &y);
      // std::cout << "TexCod: " << x << " " << y << std::endl;
      vts.push_back(Vec3(x, y, 0));
    }
    if (prefix("vn ", buf)) {
      sscanf(buf, "vn %f %f %f", &x, &y, &z);
      // std::cout << "Normal: " << Vec3(x,y,z) << std::endl;
      vns.push_back(Vec3(x, y, z));
    }

    if (prefix("f ", buf)) {
      int v0, v1, v2, n0, n1, n2, t0, t1, t2;
      Triangle *t;


      // Vertices only
      if (sscanf(buf, "f %d %d %d", &v0, &v1, &v2) == 3) {
        t = new Triangle(vs[v0-1],  vs[v1-1],  vs[v2-1]);
      }

      // Vertices + Normals
      if (sscanf(buf, "f %d//%d %d//%d %d//%d", 
                 &v0, &n0, &v1, &n1, &v2, &n2) == 6) {
        t = new Triangle( vs[v0-1],  vs[v1-1],  vs[v2-1],
                         vns[n0-1], vns[n1-1], vns[n2-1]);
      }
      
      // Vertices + Normals + Textures
      if (sscanf(buf, "f %d/%d/%d %d/%d/%d %d/%d/%d", 
                 &v0, &t0, &n0, &v1, &t1, &n1, &v2, &t2, &n2) == 9) {
        t = new Triangle( vs[v0-1],  vs[v1-1],  vs[v2-1],
                         vns[n0-1], vns[n1-1], vns[n2-1],
                         vts[t0-1], vts[t1-1], vts[t2-1]);
      }

      faces.push_back((Primitive *) t);
    }
    
  }
  printf("[+] Loaded %s; vert=%lu, face=%lu\n", fname, vs.size(), faces.size());
  return faces;
}