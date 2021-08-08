#include <core/texture.h>
#include <util/image.h>


void Texture::save(int size_x, int size_y, const char *filename) {
  Image im = Image(size_x, size_y);
  for (int x = 0; x < size_x; x++)
    for (int y = 0; y < size_y; y++) {
      float px = (float) x / (size_x - 1);
      float py = (float) y / (size_y - 1);
      im.set(x, y, get(px, py));
    }
  im.save(filename, false);
}
