#include <util/map_converter.h>
#include <core/math.h>

int wrapped_index(int x, int max) {
  if (x > max) x = x % max;
  if (x < 0) x = (x + max) % max; 
  return x;
}

Image height_to_normal_map(Image &heightIm, float strength) {
  Image normalIm = Image(heightIm.sx, heightIm.sy);

  int stride = 1;

  for (int i = 0; i < normalIm.sx; i++) {
    for (int j = 0; j < normalIm.sy; j++) {
      int im = wrapped_index(i - stride, normalIm.sx - 1);
      int ip = wrapped_index(i + stride, normalIm.sx - 1);
      int jm = wrapped_index(j - stride, normalIm.sy - 1);
      int jp = wrapped_index(j + stride, normalIm.sy - 1);

      // Get 8 neighbouring pixels
      float TL = luminance(heightIm.get(im, jm));
      float T =  luminance(heightIm.get(i , jm));
      float TR = luminance(heightIm.get(ip, jm));
      float R =  luminance(heightIm.get(ip, j ));
      float BR = luminance(heightIm.get(ip, jp));
      float B =  luminance(heightIm.get(i , jp));
      float BL = luminance(heightIm.get(im, jp));
      float L =  luminance(heightIm.get(im, j ));

      // Sobel operator
      float cx = (TR - TL) + (BR - BL) + 2*(R - L) * strength;
      float cy = (TR - BR) + (TL - BL) + 2*(T - B) * strength;

      Vec3 n = normalized(Vec3(cx, cy, 1));
      normalIm.set(i, j, inverseLerp(n, -1, 1));
    }
  }
  return normalIm;
}