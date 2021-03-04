#include "util/map_converter.h"

int addWrap(int a, int off, int max) {
  int x = a + off;
  if (x > max) x = x % max;
  if (x < 0) x = (x + max) % max; 
  return x;
}

Image convertHeightMapToNormalMap(Image &heightIm, double strength) {
  Image normalIm = Image(heightIm.sx, heightIm.sy);

  int stride = 1;

  for (int i = 0; i < normalIm.sx; i++) {
    for (int j = 0; j < normalIm.sy; j++) {
      int im = addWrap(i, -stride, normalIm.sx - 1);
      int ip = addWrap(i,  stride, normalIm.sx - 1);
      int jm = addWrap(j, -stride, normalIm.sy - 1);
      int jp = addWrap(j,  stride, normalIm.sy - 1);

      // Get 8 neighbouring pixels
      double TL = luminance(heightIm.get(im, jm));
      double T =  luminance(heightIm.get(i , jm));
      double TR = luminance(heightIm.get(ip, jm));
      double R =  luminance(heightIm.get(ip, j ));
      double BR = luminance(heightIm.get(ip, jp));
      double B =  luminance(heightIm.get(i , jp));
      double BL = luminance(heightIm.get(im, jp));
      double L =  luminance(heightIm.get(im, j ));

      // Sobel operator
      double cx = (TR - TL) + (BR - BL) + 2*(R - L) * strength;
      double cy = (TR - BR) + (TL - BL) + 2*(T - B) * strength;

      Vec n = norm(Vec(cx, cy, 1));
      normalIm.set(i, j, inverseLerp(n, -1, 1));
    }
  }
  return normalIm;
}