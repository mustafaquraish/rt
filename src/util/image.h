#ifndef __IMAGE_H__
#define __IMAGE_H__

#include <stdio.h>
#include <string.h>
#include "core/math.h"

struct Image {
  Image(int sx, int sy);
  Image(char const *fname);
  ~Image();
  void save(char const *fname, bool gammaCorrect=true);
  void set(int i, int j, const Vec& col);
  int sx, sy;
  double* data;
};

#endif // __IMAGE_H__