#include "util/obj_loader.h"

#include "stdio.h"
#include "string.h"
#include "stdlib.h"

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
  std::vector<Vec> vs;
  std::vector<Vec> vns;
  std::vector<Vec> vts;
  std::vector<Primitive *> faces;

  double x, y, z;
  // Store vertices
  while (fgets(buf, 128, f)) {
    if (prefix("v ", buf)) {
      sscanf(buf, "v %lf %lf %lf", &x, &y, &z);
      // std::cout << "Vertex: " << Vec(x,y,z) << std::endl;
      vs.push_back(Vec(x,y,z));
    }
    if (prefix("vt ", buf)) {
      sscanf(buf, "vt %lf %lf", &x, &y);
      // std::cout << "TexCod: " << x << " " << y << std::endl;
      vts.push_back(Vec(x,y,0));
    }
    if (prefix("vn ", buf)) {
      sscanf(buf, "vn %lf %lf %lf", &x, &y, &z);
      // std::cout << "Normal: " << Vec(x,y,z) << std::endl;
      vns.push_back(Vec(x,y,z));
    }

    if (prefix("f ", buf)) {
      int v0, v1, v2, n0, n1, n2, t0, t1, t2;
      sscanf(buf, "f %d/%d/%d %d/%d/%d %d/%d/%d", &v0, &t0, &n0,
                                                  &v1, &t1, &n1,
                                                  &v2, &t2, &n2);
      Triangle *t = new Triangle( vs[v0-1],  vs[v1-1],  vs[v2-1],
                                 vns[n0-1], vns[n1-1], vns[n2-1],
                                 vts[t0-1], vts[t1-1], vts[t2-1]);
      // std::cout << "Face: " << vs[v0-1] << ", " << vs[v1-1] << ", " << vs[v2-1] << std::endl;
      // std::cout << "   n: " << vns[n0-1] << ", " << vns[n1-1] << ", " << vns[n2-1] << std::endl;
      // std::cout << "    bounds: " << t->getBounds() << std::endl;
      faces.push_back((Primitive *) t);
    }
    
  }
  printf("[+] Loaded %s; vert=%lu, face=%lu\n", fname, vs.size(), faces.size());
  return faces;
}