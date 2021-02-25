#include "core/texture.h"
#include "util/image.h"

/** inlined for now **/ 

Colour Texture::get(double u, double v) {
  HitRec rec;
  rec.u = u;
  rec.v = v;
  return get(rec);
}


void Texture::saveImage(int size_x, int size_y, const char *filename) {
  Image im = Image(size_x, size_y);
  for (int x = 0; x < size_x; x++)
    for (int y = 0; y < size_y; y++) {
      double px = (double) x / (size_x - 1);
      double py = (double) y / (size_y - 1);
      im.set(x, y, get(px, py));
    }
  im.save(filename, false);
}
