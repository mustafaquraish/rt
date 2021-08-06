#include <../src/core/perlin.h>
#include <../src/util/image.h>

int main() {
  Image im = Image(512, 512);
  int pscale = 4;
  Perlin perlin = Perlin(pscale);
  for (int i = 0; i < 512; i++) {
    for (int j = 0; j < 512; j++) {
      float u = i/(512.0/pscale), v = j/(512.0/pscale), w = 1; 
      Vec3 p = Vec3(u, v, w);
      float scale = 10;
      Vec3 col = 0.5*(1 + sin(scale * p.z + 10*perlin.turb(p)));
      im.set(i, j, col);
    }
  }
  im.save("perlin.ppm", false);
}