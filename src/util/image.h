#ifndef __IMAGE_H__
#define __IMAGE_H__

#include <stdio.h>
#include <string.h>
#include "core/math.h"
#include <unordered_map>

struct Image {
  Image(int sx, int sy);
  Image(char const *fname);
  ~Image();
  
  void save(char const *fname, bool gammaCorrect=true, double exposure=1.0);
  
  void set(int i, int j, Colour col);
  void splat(int i, int j, Colour col);
  
  Colour get(int i, int j);
  Colour get(double u, double v);
  
  int sx, sy;
  double *data;
};

struct RTImageList {
  inline static std::unordered_map<std::string, Image *> imgFileMapping;

  static void registerImage(std::string filename, Image *img);
  static Image *getImage(std::string filename);

  static void cleanup();
};

#endif // __IMAGE_H__