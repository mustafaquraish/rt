#ifndef __IMAGE_H__
#define __IMAGE_H__

#include <stdio.h>
#include <string.h>
#include "core/math.h"

struct Image {
  Image(int sx, int sy);
  Image(char const *fname);
  ~Image();
  void save(char const *fname);
  void saveHDR(char const *fname);
  void set(int i, int j, const Vec& col);
  void accumHDR(int i, int j, const Vec& col);
  int sx, sy;
  double* data;
  double* weights;
};

#endif // __IMAGE_H__