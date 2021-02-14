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
  void saveHDR(char const *fname);
  
  void set(int i, int j, const Vec& col);
  void splat(int i, int j, const Vec& col);
  
  Colour get(int i, int j);
  Colour get(double u, double v);
  
  int sx, sy;
  double *data;
  double *weights;
};

#endif // __IMAGE_H__